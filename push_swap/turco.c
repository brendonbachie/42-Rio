#include "push_swap.h"

void    turco(t_elements **stack_a, t_elements **stack_b)
{
    int size;
    t_elements *min_custo;

    size = ft_lstsize(&stack_a);

    pb(stack_a, stack_b);

    while (ft_lstsize(stack_a) > 3)
    {
        index(*stack_a);
        index_b(*stack_a, *stack_b);
        alvo(*stack_a, *stack_b);
        custo(*stack_a, *stack_b);
        custo_total(*stack_a);
        min_custo = push_min_custo(*stack_a);
        mov_final(stack_a, stack_b, min_custo);
    }
    three(stack_a);
    while(stack_b)
    {
        index(*stack_b);
        index_b(*stack_b, *stack_a);
        volta(*stack_a, *stack_b);
        custo(*stack_b, *stack_a);
        custo_total(*stack_b);
        min_custo = push_min_custo(*stack_b);
        mov_final2(stack_a, stack_b, min_custo);
    }
}