#include "push_swap.h"

t_elements	*push_min_custo(t_elements *stack_a)
{
	t_elements	*min_custo;
	t_elements	*temp;

	temp = stack_a;
	min_custo = stack_a;
	while (temp)
	{
		if (temp->custo_total < min_custo->custo_total)
			min_custo = temp;
		temp = temp->next;
	}
	return (min_custo);
}