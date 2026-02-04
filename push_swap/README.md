*This project has been created as part of the 42 curriculum by bgomes-b.*

# Descrição

O projeto Push-swap pede um programa em C que receba um conjunto de inteiros e escreva uma sequência de instruções que os ordene de forma crescente. Para isso, o programa está autorizado a utilizar apenas um conjunto específico de operações e duas pilhas. 

A grande questão é que, embora não se exija eficiência do programa Push_swap em si, a sequência de instruções necessária para ordenar os números inteiros deve ser a menor possível. 

Este projeto serve como uma introdução prática ao conceito de complexidade de algoritmos e exige rigor na manipulação de memória e eficiência lógica.


# Instruções
## Compilação

O projeto deve ser compilado através de um Makefile, que não deve realizar "relink" desnecessário. Os seguintes comandos estão disponíveis:

make: Compila o programa principal push_swap.

make bonus: (Opcional) Compila o programa checker.

make clean: Remove os arquivos objetos.

make fclean: Remove os arquivos objetos e o executável.

make re: Recompila todo o projeto.

## Execução

O programa recebe como argumento uma lista de inteiros que representam a pilha A. O primeiro argumento fornecido deve ficar no topo da pilha.

Exemplos de uso:  
./push_swap 2 1 3 6 5 8  
ARG="4 67 3 87 23"; ./push_swap $ARG
ARG=$(shuf -i 0-10000 -n 500 | tr '\n' ' '); ./push_swap $ARG     --> passa 500 inteiros aleatórios entre 0 e 10000

O programa exibirá uma lista de instruções separadas por uma quebra de linha.

Exemplo de saída:  
$>./push_swap 2 1 3 6 5 8  
sa  
pb  
pb  
pb  
sa  
pa  
pa  
pa  

## Tratamento de Erros

- Se nenhum argumento é passado, o procgrama se encerra e nada é exibido.
- Caso os argumentos não sejam passíveis de serem convertidos em inteiros, o programa deve se encerrar e exibir mensagem de erro. 
- Não pode haver números duplicados entre os argumentos.

- Exemplos de entradas válidas:  
./push_swap -3 41 0 6 12  
./push_swap "-3 41 0" 6 12
  
- Exemplos de entradas inválidas:  
./push_swap 2 1 3 6 5 a  
./push_swap 25 -1 --5  
./push_swap -3 41 0 6 12 12

## Regras e Operações

As operações permitidas para manipular as pilhas incluem:

- Trocar (Swap): sa, sb, ss (troca os 2 primeiros elementos).

- Enviar (Push): pa, pb (move o elemento do topo de uma pilha para a outra).

- Rotacionar (Rotate): ra, rb, rr (desloca todos os elementos para cima; o primeiro vira o último).

- Rotacionar ao contrário (Reverse Rotate): rra, rrb, rrr (desloca todos os elementos para baixo; o último vira o primeiro).

OBS: o último caractere das operações se refere à pilha em que é aplicado, sendo que o 'r' e o 's' indicam a operação aplicada em ambas as pilhas ao mesmo tempo.

## Benchmark (Metas de Desempenho)

Para a validação completa do projeto, o algoritmo deve respeitar os seguintes limites de operações:

- 100 números: Menos de 700 operações.
- 500 números: Menos de 5500 operações.
  

## Bônus

O bônus deste projeto consiste em criar um programa chamado "checker" que valide ou reprove o conjunto de instruções gerado pelo programa Push_swap para um determinado conjunto de argumentos passados. Em caso de sucesso, o programa imprimirá "OK". Caso contrário, exibirá "KO".

Uso:

(1ª Opção) ./checker 3 2 1 0  -->  O programa, então, pedirá a sequência de instruções (previamente obtida do programa Push_swap) para ordenar a entrada e, então, testa as instruções.

(2ª Opção) ./push_swap 3 2 1 0 | ./checker 3 2 1 0  -->  O checker já recebe a sequência de instruções do programa Push_swap e testa as instruções.


# Recursos
## Referências e Documentação

- Algoritmos de Ordenação (Position Sort).

- Documentação da linguagem C e gestão de pilhas (Stacks).

- Complexidade de Tempo e Espaço (Big O Notation).
  

## Uso de IA

Neste projeto, a Inteligência Artificial foi utilizada para as seguintes tarefas:

Exemplo: Auxílio no design do parser de argumentos para garantir que apenas números inteiros válidos sejam aceitos.

Exemplo: Geração de ideias para casos de teste e verificação de lógica de ordenação com colegas de turma.

Nota: Todo o código gerado ou sugerido por ferramentas de IA foi revisado, testado e compreendido integralmente, conforme as diretrizes do capítulo de "AI Instructions" do projeto.
