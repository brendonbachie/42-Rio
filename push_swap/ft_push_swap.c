#include "push_swap.h"

void    ate3(t_elements **stack_a)
{
    int size;
    size = ft_lstsize(stack_a);
    if (size <= 1)
        return ;
    if (size == 2)
    {
        if ((*stack_a)->number > (*stack_a)->next->number)
            sa(stack_a);
        return ;
    }
    if (size == 3)
    {
        three(stack_a);
        return ;
    }
}