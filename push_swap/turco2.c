#include "push_swap.h"

void    turco(t_elements **stack_a, t_elements **stack_b)
{
    t_elements *min_custo;
    t_elements *menor;

    int median = find_median(*stack_a);
    int target = ft_lstsize(stack_a) / 2;

    while (target)
    {
        if ((*stack_a)->number <= median)
        {
            pb(stack_a, stack_b);
            if ((*stack_b)->number < median)
                rb(stack_b);
            target--;
        }
        else
            ra(stack_a);
    }

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

int cmp_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int find_median(t_elements *stack)
{
    int size;
    int *arr;
    int i;
    t_elements *tmp;
    int median;

    size = ft_lstsize(&stack);
    arr = malloc(sizeof(int) * size);
    if (!arr)
        return (0);

    tmp = stack;
    i = 0;
    while (tmp)
    {
        arr[i++] = tmp->number;
        tmp = tmp->next;
    }

    qsort(arr, size, sizeof(int), cmp_int);

    median = arr[size / 2];
    free(arr);
    return (median);
}