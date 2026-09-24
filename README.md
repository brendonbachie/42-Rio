# 42-Rio

Coleção dos projetos do currículo da 42 Rio de Brendon Gomes Bachie (login `bgomes-b`), organizados por linguagem.

## Projetos

| Projeto | Descrição | Linguagem/stack |
|---|---|---|
| [Get_Next_Line](C/Get_Next_Line) | Função que lê uma linha por vez de um file descriptor, incluindo versão bônus com múltiplos fds simultâneos. | C |
| [born](C/born) [born2beroot] | Documentação da configuração de uma VM Rocky Linux hardenizada (partições com LVM, SELinux enforcing, políticas de senha, SSH sem root, firewall). | Rocky Linux / administração de sistemas |
| [codexion](C/codexion) | Simulação de concorrência inspirada no problema dos filósofos, com coders competindo por dongles USB para compilar; implementada com threads POSIX e mutexes, incluindo uma thread monitora de burnout. | C (pthreads) |
| [inception](C/inception) | Infraestrutura web estilo LEMP com Docker Compose: containers NGINX, WordPress+php-fpm e MariaDB, cada um com Dockerfile próprio sobre Alpine, volumes nomeados e secrets do Docker. | Docker / Docker Compose |
| [libft](C/libft) | Biblioteca própria com reimplementações de funções da libc (string, memória, listas ligadas, conversões) para uso nos demais projetos da 42. | C |
| [myprintf](C/myprintf) [ft_printf] | Reimplementação da função `printf`, com parsing de formato e tratamento de argumentos variádicos, gerando uma biblioteca estática. | C |
| [push_swap](C/push_swap) | Programa que ordena uma pilha de inteiros usando um conjunto restrito de operações sobre duas pilhas, buscando a menor sequência possível de instruções. | C |
| [RAG](Python/RAG) | Sistema de Retrieval-Augmented Generation que indexa um repositório de código (vLLM), recupera trechos relevantes para uma pergunta e gera resposta com um modelo local (`Qwen/Qwen3-0.6B`), medindo qualidade da recuperação via recall@k. | Python |
| [cmm](Python/cmm) [call me maybe] | Faz um modelo de linguagem pequeno produzir chamadas de função em JSON estruturado e válido a partir de um pedido em linguagem natural, usando decodificação restrita (constrained decoding) sobre os logits. | Python |
| [fly-in](Python/fly-in) | Simulador de roteamento de uma frota de drones sobre um grafo de zonas e conexões, respeitando limites de ocupação/capacidade e custos de movimento por tipo de zona; grafo e Dijkstra implementados do zero, com log colorido e replay gráfico opcional. | Python |

## Como compilar/rodar

Os projetos em C usam `make` (padrão `make`, `make bonus`, `make clean`, `make fclean`, `make re`, quando aplicável). `inception` usa `make` para orquestrar o `docker compose`.

Os projetos em Python (`RAG`, `cmm`, `fly-in`) usam `make install` para instalar dependências — `RAG` e `cmm` via [`uv`](https://docs.astral.sh/uv/), `fly-in` via `pip`/`requirements.txt` — e expõem alvos próprios como `make run` ou `make lint`; veja o README de cada pasta para detalhes.

## Licença

MIT, veja [LICENSE](LICENSE).
