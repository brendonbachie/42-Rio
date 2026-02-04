/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cost_calc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 19:05:28 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/02 19:05:28 by bgomes-b         ###   ########.fr       */
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
	int	cost_a;
	int	cost_b;

	while (stack_a)
	{
		cost_a = stack_a->cost_a;
		cost_b = stack_a->cost_b;
		if ((cost_a >= 0 && cost_b >= 0) || (cost_a < 0 && cost_b < 0))
		{
			if (ft_mod(cost_a) > ft_mod(cost_b))
				stack_a->total_cost = ft_mod(cost_a);
			else
				stack_a->total_cost = ft_mod(cost_b);
		}
		else
			stack_a->total_cost = ft_mod(cost_a) + ft_mod(cost_b);
		stack_a = stack_a->next;
	}
}

t_node	*lowest_cost(t_node *stack_b)
{
	t_node	*low_cost;
	t_node	*temp;

	temp = stack_b;
	low_cost = stack_b;
	while (temp)
	{
		if (temp->total_cost < low_cost->total_cost)
			low_cost = temp;
		temp = temp->next;
	}
	return (low_cost);
}

int	ft_mod(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}
