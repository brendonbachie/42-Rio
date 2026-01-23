#include "push_swap.h"

t_elements	*ft_lstnew(int number)
{
	t_elements	*new;
	
	new = malloc(sizeof(t_elements));
	if (!new)
		return (NULL);
	new->number = number;
	new->next = NULL;
	
	return (new);
}
