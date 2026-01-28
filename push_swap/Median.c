#include "push_swap.h"

void	find_index(t_node *stack_a)
{
	t_node *cur;
	t_node *cmp;
	int index;

	cur = stack_a;
	while (cur)
	{
		index = 0;
		cmp = stack_a;
		while (cmp)
		{
			if (cmp->number < cur->number)
				index++;
			cmp = cmp->next;
		}
		cur->index = index;
		cur = cur->next;
	}
}