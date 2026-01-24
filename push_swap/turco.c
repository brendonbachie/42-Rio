#include "push_swap.h"

void    turco(t_elements **stack_a, t_elements **stack_b)
{
    int size;

    size = ft_lstsize(&stack_a);

    pb(stack_a, stack_b);

    while (ft_lstsize(stack_a) > 3)
    {
        index(*stack_a);
        index_b(*stack_a, *stack_b);
        alvo(*stack_a, *stack_b);
        custo(*stack_a, *stack_b);
        custo_total(*stack_a);
        push_min_custo(stack_a, stack_b);
    }