#include "push_swap.h"

void    mov_final2(t_elements **stack_a, t_elements **stack_b, t_elements *min_custo)
{
        int custo_a;
        int custo_b;

        custo_a = min_custo->custo_a;
        custo_b = min_custo->custo_b;

        while (custo_a > 0 && custo_b > 0)
        {
                rr(stack_a, stack_b);
                custo_a--;
                custo_b--;
        }
        while (custo_a < 0 && custo_b < 0)
        {
                rrr(stack_a, stack_b);
                custo_a++;
                custo_b++;
        }
        while (custo_a > 0)
        {
                ra(stack_a);
                custo_a--;
        }
        while (custo_a < 0)
        {
                rra(stack_a);
                custo_a++;
        }
        while (custo_b > 0)
        {
                rb(stack_b);
                custo_b--;
        }
        while (custo_b < 0)
        {
            rrb(stack_b);
                custo_b++;
        }
        pa(stack_a, stack_b);      
}
