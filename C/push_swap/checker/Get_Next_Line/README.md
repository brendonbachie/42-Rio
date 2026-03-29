*This project has been created as part of the 42 curriculum by <bgomes-b>.*

# Get Next Line

## Description

O projeto get_next_line tem como objetivo implementar uma função em C capaz de ler uma linha completa de um file descriptor, retornando-a a cada chamada sucessiva da função.

A principal proposta do projeto é aprofundar o entendimento sobre:
- Leitura de arquivos em baixo nível (read)
- Gerenciamento de memória dinâmica
- Uso de variáveis estáticas
- Manipulação de strings
- Tratamento de múltiplos file descriptors (bônus)

A função deve retornar uma linha por vez, incluindo o caractere de nova linha (\n) quando presente, até que o final do arquivo (EOF) seja alcançado.

---

## Instructions

### Compilation

O projeto não gera um executável por si só, mas sim uma função que deve ser compilada junto ao seu programa de teste.

Exemplo de compilação:

cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c main.c

Para a versão bônus (com múltiplos file descriptors):

cc -Wall -Wextra -Werror get_next_line_bonus.c get_next_line_utils_bonus.c main.c

### Usage

Exemplo simples de uso:

int     fd;
char    *line;

fd = open("file.txt", O_RDONLY);
while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
close(fd);

---

## Algorithm Explanation and Justification

O algoritmo utilizado na função get_next_line baseia-se nos seguintes passos:

1. Leitura incremental  
Utiliza-se a função read() para ler o conteúdo do file descriptor em blocos de tamanho BUFFER_SIZE.

2. Armazenamento persistente  
Uma variável estática é usada para armazenar os dados lidos que ainda não foram retornados, permitindo que o estado da leitura seja preservado entre chamadas da função.

3. Busca pelo caractere de nova linha (\n)  
Após cada leitura, o conteúdo armazenado é analisado para verificar se já existe uma linha completa.

4. Extração da linha  
Quando um \n é encontrado (ou EOF é atingido), a linha é extraída e retornada, enquanto o restante do conteúdo é mantido para a próxima chamada.

5. Gerenciamento de memória  
Toda a memória alocada dinamicamente é cuidadosamente controlada para evitar vazamentos, garantindo que cada chamada retorne uma string corretamente alocada.

Justificativa

Essa abordagem foi escolhida porque:
- Respeita a limitação de leitura em blocos
- Evita reler dados desnecessariamente
- Permite lidar corretamente com arquivos grandes
- Facilita a extensão para múltiplos file descriptors (bônus)
- Mantém o código modular e legível

---

## Resources

Referências Técnicas
- Manual da função read (man 2 read)
- Manual de file descriptors (man 2 open, man 2 close)
- Documentação oficial da linguagem C
- Norminette da 42
- Materiais internos da 42 sobre variáveis estáticas e alocação dinâmica

Uso de Inteligência Artificial

A inteligência artificial foi utilizada exclusivamente como ferramenta de apoio, com as seguintes finalidades:
- Revisão de explicações teóricas sobre o funcionamento do algoritmo
- Melhoria da clareza na documentação (README)
- Auxílio na organização textual e explicações conceituais

Nenhum código final do projeto foi gerado automaticamente por IA. Toda a implementação foi escrita, testada e validada manualmente, em conformidade com as regras da 42.

