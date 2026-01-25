#include "push_swap.h"

void    ra(t_elements **stack_a)
{
    t_elements *temp;
    t_elements *first;

    if (stack_a == NULL || *stack_a == NULL || (*stack_a)->next == NULL)
        return;

    first = *stack_a;
    *stack_a = (*stack_a)->next;
    first->next = NULL;

    temp = *stack_a;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = first;
    //write(1, "ra\n", 3);
}
void    rb(t_elements **stack_b)
{
    t_elements *temp;
    t_elements *first;

    if (stack_b == NULL || *stack_b == NULL || (*stack_b)->next == NULL)
        return;

    first = *stack_b;
    *stack_b = (*stack_b)->next;
    first->next = NULL;

    temp = *stack_b;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = first;
    //write(1, "rb\n", 3);
}
void    rr(t_elements **stack_a, t_elements **stack_b)
{
    ra(stack_a);
    rb(stack_b);
}