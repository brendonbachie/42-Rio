// =============================================
// FUNÇÕES
// ============================================

function trocar_code_por_token(string $code): string {
    $response = http_post('https://api.intra.42.fr/oauth/token', [
        'grant_type'    => 'authorization_code',
        'client_id'     => CLIENT_ID,
        'client_secret' => CLIENT_SECRET,
        'code'          => $code,
        'redirect_uri'  => REDIRECT_URI,
    ]);

    $data = json_decode($response, true);

    if (empty($data['access_token'])) {
        erro('Falha ao obter token da 42.');
    }

    return $data['access_token'];
}

function buscar_usuario(string $token): array {
    $response = http_get('https://api.intra.42.fr/v2/me', $token);
    $data     = json_decode($response, true);

    if (empty($data['login'])) {
        erro('Falha ao buscar dados do usuário.');
    }

    return $data;
}

function liberar_no_pfsense(string $action, string $redirurl, string $zone): void {
    // Faz POST diretamente pro pfSense com accept=Continue
    $ch = curl_init($action);
    curl_setopt_array($ch, [
        CURLOPT_POST           => true,
        CURLOPT_POSTFIELDS     => http_build_query([
            'accept'   => 'Continue',
            'redirurl' => $redirurl,
            'zone'     => $zone,
        ]),
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_TIMEOUT        => 30,
        CURLOPT_SSL_VERIFYPEER => false,
        CURLOPT_FOLLOWLOCATION => false,
    ]);
    curl_exec($ch);
    curl_close($ch);
}

function salvar_metrica(string $username, string $ip): void {
    $linha = implode(',', [date('Y-m-d H:i:s'), $username, $ip]) . PHP_EOL;
    file_put_contents(__DIR__ . '/logins.csv', $linha, FILE_APPEND | LOCK_EX);
}

function http_post(string $url, array $fields): string {
    $ch = curl_init($url);
    curl_setopt_array($ch, [
        CURLOPT_POST           => true,
        CURLOPT_POSTFIELDS     => http_build_query($fields),
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_TIMEOUT        => 30,
    ]);
    $res = curl_exec($ch);
    curl_close($ch);
    return $res ?: '';
}

function http_get(string $url, string $bearer): string {
    $ch = curl_init($url);
    curl_setopt_array($ch, [
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_TIMEOUT        => 30,
        CURLOPT_HTTPHEADER     => ["Authorization: Bearer $bearer"],
    ]);
    $res = curl_exec($ch);
    curl_close($ch);
    return $res ?: '';
}

function erro(string $msg): never {
    http_response_code(500);
    echo '<h2>Erro ao autenticar</h2><p>' . htmlspecialchars($msg) . '</p>';
    exit;
}

// =============================================
// FLUXO PRINCIPAL
// =============================================

if (empty($_GET['code'])) {
    erro('Parâmetro "code" ausente.');
}
// DEBUG TEMPORÁRIO
file_put_contents(__DIR__ . '/debug.log',
    date('H:i:s') . ' | IP: ' . $_SERVER['REMOTE_ADDR'] .
    ' | code: ' . substr($_GET['code'] ?? '', 0, 10) .
    ' | state: ' . ($_GET['state'] ?? 'none') . "\n",
    FILE_APPEND
);

// Recupera os parâmetros do pfSense que vieram no state
$state   = json_decode(urldecode($_GET['state'] ?? '{}'), true);
$action  = $state['action']   ?? '';
$redirurl = $state['redirurl'] ?? 'http://www.google.com';
$zone    = $state['zone']     ?? '';

// 1. Troca o code pelo token
$token = trocar_code_por_token($_GET['code']);

// 2. Busca dados do usuário
$usuario  = buscar_usuario($token);
$username = $usuario['login'];
$ip       = $_SERVER['REMOTE_ADDR'];

// 3. Libera no pfSense
if ($action) {
    liberar_no_pfsense($action, $redirurl, $zone);
}

// 4. Salva métrica
salvar_metrica($username, $ip);

// 5. Redireciona pro destino original
header('Location: ' . $redirurl);
exit;
```

---

## O que falta configurar

Substitui nos dois arquivos:
```
SEU_CLIENT_ID      → client_id da sua app na 42
SEU_CLIENT_SECRET  → client_secret da sua app na 42
SEU_NGROK          → URL gerada pelo ngrok
