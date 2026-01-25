#include "push_swap.h"

t_elements *maior_valor(t_elements *stack)
{
    t_elements *temp;
    t_elements *maior;

    temp = stack;
    maior = stack;
    while (temp)
    {
        if (temp->number > maior->number)
            maior = temp;
        temp = temp->next;
    }
    return (maior);
}

void alvo(t_elements *stack_a, t_elements *stack_b)
{
    t_elements *a;
    t_elements *b;
    t_elements *best;
    long best_value;

    a = stack_a;
    while (a)
    {
        best = NULL;
        best_value = -2147483649;
        b = stack_b;
        while (b)
        {
            if (b->number < a->number && b->number > best_value)
            {
                best_value = b->number;
                best = b;
            }
            b = b->next;
        }
        if (!best)
            best = maior_valor(stack_b);
        a->alvo = best;
        a = a->next;
    }
}



void    index_b(t_elements *stack_a, t_elements *stack_b)
{
    int i;
    t_elements *temp_a;
    t_elements *temp_b;

    temp_a = stack_a;
    while (temp_a)
    {
        i = 0;
        temp_b = stack_b;
        while (temp_b)
        {
            if (temp_a->alvo && temp_a->alvo == temp_b)
            {
                temp_a->index_b = i;
                break;
            }
            temp_b = temp_b->next;
            i++;
        }
        temp_a = temp_a->next;
    }
}

void    indice(t_elements *stack)
{
    int i;
    t_elements *temp;

    temp = stack;
    i = 0;
    while (temp)
    {
        temp->index = i;
        i++;
        temp = temp->next;
    }
}

void    custo(t_elements *stack_a, t_elements *stack_b)
{
    t_elements *temp;
    int size_a;
    int size_b;

    size_a = ft_lstsize(&stack_a);
    size_b = ft_lstsize(&stack_b);
    temp = stack_a;

    while (temp)
    {
        if (temp->index <= size_a / 2)
            temp->custo_a = temp->index;
        else
            temp->custo_a = temp->index - size_a;

        if (temp->index_b <= size_b / 2)
            temp->custo_b = temp->index_b;
        else
            temp->custo_b = temp->index_b - size_b;
        
        temp = temp->next;
    }
}

void custo_total(t_elements *stack)
{
    t_elements *temp;
    int custo_a;
    int custo_b;

    temp = stack;
    while (temp)
    {
        custo_a = temp->custo_a;
        custo_b = temp->custo_b;
        if ((custo_a >= 0 && custo_b >= 0) || (custo_a < 0 && custo_b < 0))
        {
            if (custo_a >= 0 && custo_b >= 0)
                if ((custo_a) > (custo_b))
                    temp->custo_total = (custo_a);
                else
                    temp->custo_total = (custo_b);
            else 
                if ((custo_a) < (custo_b))
                    temp->custo_total = -(custo_a);
                else
                    temp->custo_total = -(custo_b);
        }
        else
            temp->custo_total = ft_mod(custo_a) + ft_mod(custo_b);
        temp = temp->next;
    }
}