#include "push_swap.h"

int	ft_lstsize(t_elements **lst)
{
	t_elements *temp;
	int size;

    size = 0;
    temp = *lst;
    while (temp != NULL)
    {
        temp = temp->next;
        size++;
    }
	return (size);
}