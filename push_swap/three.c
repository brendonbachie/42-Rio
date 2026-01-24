#include "push_swap.h"

void    three(t_elements **stack_a)
{
   if ((*stack_a)->number > (*stack_a)->next->number && (*stack_a)->next->number > (*stack_a)->next->next->number)
        {
            sa(stack_a);
            rra(stack_a);
        }
        else if ((*stack_a)->number > (*stack_a)->next->number && (*stack_a)->next->number < (*stack_a)->next->next->number)
        {
            sa(stack_a);
        }
        else if ((*stack_a)->number < (*stack_a)->next->number && (*stack_a)->next->number > (*stack_a)->next->next->number)
        {
            sa(stack_a);
            ra(stack_a);
        }
}