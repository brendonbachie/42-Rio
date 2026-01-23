#include "push_swap.h"

int ft_isnum(char **str)
{
    int i;
    int a;

    i = 0;
    a = 0;
    while (str[i] != '\0')
    {
        if (str[i][a] == ' ')
            a++;
        if ((str[i][a] == '-' || str[i][a] == '+') 
            && !(str[i][a - 1] == ' ' || a == 0))
            return (0);
        else if (str[i][a] == '-' || str[i][a] == '+')
            a++;
        if ((str[i][a] < '0' || str[i][a] > '9'))
            return (0);
        a++;
        if (str[i][a] == '\0')
        {
            i++;
            a = 0;
        }
    }
    return (1);
}
