#include "push_swap.h"

void    turco(t_elements **stack_a, t_elements **stack_b)
{
    t_elements *min_custo;
    t_elements *menor;

    pb(stack_a, stack_b);
    if(ft_lstsize(stack_a) > 3)
        pb(stack_a, stack_b);
    while (ft_lstsize(stack_a) > 3)
    {
        indice(*stack_a);
        alvo(*stack_a, *stack_b);
        index_b(*stack_a, *stack_b);
        custo(*stack_a, *stack_b);
        custo_total(*stack_a);
        min_custo = push_min_custo(*stack_a);
        mov_final(stack_a, stack_b, min_custo);
        //pb(stack_a, stack_b);
    }
    three(stack_a);
    while(ft_lstsize(stack_b) > 0)
    {
        indice(*stack_b);
        volta(*stack_a, *stack_b);
        index_b(*stack_b, *stack_a);
        custo(*stack_b, *stack_a);
        custo_total(*stack_b);
        min_custo = push_min_custo(*stack_b);
        mov_final2(stack_b, stack_a, min_custo);
    }
    menor = menor_valor(*stack_a);
    indice(*stack_a);
    if (menor->index <= ft_lstsize(stack_a) / 2)
    {
        while ((*stack_a)->number != menor->number)
        {
            ra(stack_a);
            write(1, "ra\n", 3);
        }
    }
    else
        while ((*stack_a)->number != menor->number)
        {

            rra(stack_a);
            write(1, "rra\n", 4);
        }
    free(*stack_b);
}