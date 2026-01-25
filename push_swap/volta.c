#include "push_swap.h"

void    volta(t_elements *stack_a, t_elements *stack_b)
{
    t_elements *temp_a;
    t_elements *temp_b;
    t_elements *alvo;
    long best_value;

    temp_b = stack_b;
    while (temp_b)
    {
        alvo = NULL;
        best_value = -2147483649;
        temp_a = stack_a;
        while (temp_a)
        {
            if (temp_a->number < temp_b->number && temp_a->number > best_value)
            {
                best_value = temp_a->number;
                alvo = temp_a;
            }
            temp_a = temp_a->next;
        }
        if (!alvo)
            alvo = maior_valor(stack_a);
        temp_b->alvo = alvo;
        temp_b = temp_b->next;
    }
}