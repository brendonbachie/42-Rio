/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculos.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:00:40 by marvin            #+#    #+#             */
/*   Updated: 2026/01/25 17:45:10 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	custo(t_elements *stack_a, t_elements *stack_b)
{
	t_elements	*temp;
	int			size_a;
	int			size_b;

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

void	custo_total(t_elements *stack)
{
	t_elements	*temp;

	temp = stack;
	soma_custos(temp);
}

void	soma_custos(t_elements *stack_a)
{
	while (stack_a)
	{
		if ((stack_a->custo_a >= 0 && stack_a->custo_b >= 0)
			|| (stack_a->custo_a < 0 && stack_a->custo_b < 0))
		{
			if (stack_a->custo_a >= 0 && stack_a->custo_b >= 0)
			{
				if ((stack_a->custo_a) > (stack_a->custo_b))
					stack_a->custo_total = (stack_a->custo_a);
				else
					stack_a->custo_total = (stack_a->custo_b);
			}
			else
			{
				if ((stack_a->custo_a) < (stack_a->custo_b))
					stack_a->custo_total = -(stack_a->custo_a);
				else
					stack_a->custo_total = -(stack_a->custo_b);
			}
		}
		else
			stack_a->custo_total = ft_mod(stack_a->custo_a)
				+ ft_mod(stack_a->custo_b);
		stack_a = stack_a->next;
	}
}
