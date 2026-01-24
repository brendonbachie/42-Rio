#include "push_swap.h"

void ft_lstclear(t_elements **lst)
{
    t_elements *tmp;

    if (!lst)
        return;
    while (*lst)
    {
        tmp = (*lst)->next;
        free(*lst);
        *lst = tmp;
    }
    *lst = NULL;
}
