#include "push_swap.h"

void    sa(t_elements **stack_a)
{
    t_elements *temp;

    if (stack_a == NULL || *stack_a == NULL || (*stack_a)->next == NULL)
        return;

    temp = (*stack_a)->next;
    (*stack_a)->next = temp->next;
    temp->next = *stack_a;
    *stack_a = temp;
    write(1, "sa\n", 3);
}

void    sb(t_elements **stack_b)
{
    t_elements *temp;

    if (stack_b == NULL || *stack_b == NULL || (*stack_b)->next == NULL)
        return;

    temp = (*stack_b)->next;
    (*stack_b)->next = temp->next;
    temp->next = *stack_b;
    *stack_b = temp;
    write(1, "sb\n", 3);
}

void    ss(t_elements **stack_a, t_elements **stack_b)
{
    sa(stack_a);
    sb(stack_b);
    write(1, "ss\n", 3);
}

void    pa(t_elements **stack_a, t_elements **stack_b)
{
    t_elements *temp;

    if (stack_b == NULL || *stack_b == NULL)
        return;

    temp = *stack_b;
    *stack_b = (*stack_b)->next;
    temp->next = *stack_a;
    *stack_a = temp;
    write(1, "pa\n", 3);
}

void    pb(t_elements **stack_a, t_elements **stack_b)
{
    t_elements *temp;

    if (stack_a == NULL || *stack_a == NULL)
        return;

    temp = *stack_a;
    *stack_a = (*stack_a)->next;
    temp->next = *stack_b;
    *stack_b = temp;
    write(1, "pb\n", 3);
}