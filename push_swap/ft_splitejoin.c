#include "push_swap.h"

char   **ft_splitejoin(int quantidade, char **argumentos)
{
    char    **resultado;
    char    *temp;
    int     i;

    i = 1;
    temp = calloc(1, 1);
    resultado = NULL;
    while (i < quantidade)
    {
        temp = ft_strjoin(&temp, argumentos[i]);
        if (i + 1 < quantidade)
            temp = ft_strjoin(&temp, " ");
        i++;
    }
    resultado = ft_split(temp, ' ');
    free(temp);
    return (resultado);
}