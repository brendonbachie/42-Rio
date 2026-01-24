#include "push_swap.h"

t_elements *ft_lists(char **argumentos)
{
    t_elements *head;
    t_elements *new;
    int i;

    i = 0;
    head = NULL;
   
    while (argumentos[i])
    {
        new = ft_lstnew(atoi(argumentos[i]));
        if (!new)
        {
            ft_lstclear(&head);
            return (NULL);
        }
        ft_lstadd_back(&head, new);
        i++;
    }
    return (head);
}