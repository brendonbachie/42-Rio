#include "push_swap.h"

void ft_lstadd_back(t_elements **lst, t_elements *new)
{
    t_elements *tmp;

    if (!lst || !new)
        return;
    if (!*lst)
    {
        *lst = new;
        return;
    }
    tmp = *lst;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}
