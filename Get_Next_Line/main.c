#include <fcntl.h>   // Para 'open' e a constante 'O_RDONLY'
#include <unistd.h>  // Para funções de baixo nível como read e close
#include <stdio.h>   // Para 'printf'
#include <stdlib.h>  // Para 'free'

// Esta linha declara a sua função get_next_line para o compilador
char *get_next_line(int fd);

int main(void)
{
    int    fd1;
    char *linha1;
    char *linha2;
    int    fd2;

    fd1 = open("teste1.txt", O_RDONLY);
    fd2 = open("teste2.txt", O_RDONLY);

    printf("%d\n%d\n", fd1, fd2);
    
    // LINHA 1
    linha1 = get_next_line(fd1);
    if (linha1) printf("[%s]", linha1);
    free(linha1);

    linha2 = get_next_line(fd2);
    if (linha2) printf("[%s]", linha2);
    free(linha2);

    // LINHA 2
    linha1 = get_next_line(fd1);
    if (linha1) printf("[%s]", linha1);
    free(linha1);
    
    linha2 = get_next_line(fd2);
    if (linha2) printf("[%s]", linha2);
    free(linha2);

    // LINHA 3
    linha1 = get_next_line(fd1);
    if (linha1) printf("[%s]", linha1);
    free(linha1);
    
    linha2 = get_next_line(fd2);
    if (linha2) printf("[%s]", linha2);
    free(linha2);
    
    // CHAMADA QUE RETORNA NULL
    linha1 = get_next_line(fd1);
    if (linha1) printf("ERRO: %s", linha1); // Não deve imprimir
    free(linha1);
    
    return (0);
}
