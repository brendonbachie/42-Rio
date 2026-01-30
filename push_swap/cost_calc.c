/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculos.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:00:40 by marvin            #+#    #+#             */
/*   Updated: 2026/01/25 21:32:27 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	cost(t_node *stack_a, t_node *stack_b)
{
	t_node	*temp;
	int		size_a;
	int		size_b;

	size_a = ft_lstsize(&stack_a);
	size_b = ft_lstsize(&stack_b);
	temp = stack_a;
	while (temp)
	{
		if (temp->index_node < size_a / 2)
			temp->cost_a = temp->index_node;
		else
			temp->cost_a = temp->index_node - size_a;
		if (temp->target_index < size_b / 2)
			temp->cost_b = temp->target_index;
		else
			temp->cost_b = temp->target_index - size_b;
		temp = temp->next;
	}
}

void	total_cost(t_node *stack_a)
{
	while (stack_a)
	{
		if ((stack_a->cost_a >= 0 && stack_a->cost_b >= 0)
			|| (stack_a->cost_a < 0 && stack_a->cost_b < 0))
		{
			if (ft_mod(stack_a->cost_a) > ft_mod(stack_a->cost_b))
				stack_a->total_cost = ft_mod(stack_a->cost_a);
			else
				stack_a->total_cost = ft_mod(stack_a->cost_b);
		}
		else
			stack_a->total_cost = ft_mod(stack_a->cost_a)
				+ ft_mod(stack_a->cost_b);
		stack_a = stack_a->next;
	}
}
