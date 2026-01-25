#include "push_swap.h"

void    three(t_elements **stack_a)
{
   if ((*stack_a)->number > (*stack_a)->next->number)
    {
        if ((*stack_a)->number > (*stack_a)->next->next->number)
        {
            ra(stack_a);
            if((*stack_a)->number > (*stack_a)->next->number)
                sa(stack_a);
        }
        else
            sa(stack_a);
    }
    else if ((*stack_a)->next->number > (*stack_a)->next->next->number)
    {
        rra(stack_a);
        if((*stack_a)->number > (*stack_a)->next->number)
            sa(stack_a);
    }
}