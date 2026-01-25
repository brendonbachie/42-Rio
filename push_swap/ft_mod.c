#include "push_swap.h"

int ft_mod(int a)
{
    if (a < 0)
        return (-a);
    return (a);
}

t_elements *menor_valor(t_elements *stack_a)
{
    t_elements *menor;
    menor = stack_a;
    while (stack_a != NULL)
    {
        if (stack_a->number < menor->number)
            menor = stack_a;
        stack_a = stack_a->next;
    }
    return (menor);
}