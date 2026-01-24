#include "push_swap.h"

void    volta(t_elements **stack_a, t_elements **stack_b)
{
    t_elements *temp_a;
    t_elements *temp_b;
    t_elements *alvo;

    temp_a = *stack_a;
    temp_b = *stack_b;
    while (temp_b)
    {
        alvo = NULL;
        temp_a = *stack_a;
        while (temp_a)
        {
            if (temp_b->number < temp_a->number && (alvo == NULL || temp_a->number < alvo->number))
                alvo = temp_a;
            temp_a = temp_a->next;
        }
        temp_b->alvo = alvo;
        temp_b = temp_b->next;
    }
}