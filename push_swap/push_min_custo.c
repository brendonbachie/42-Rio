/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lowest_cost.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:07:06 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 20:38:32 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*lowest_cost_a(t_node *stack_a)
{
	t_node	*low_cost;
	t_node	*temp;

	temp = stack_a;
	low_cost = stack_a;
	while (temp)
	{
		if (temp->total_cost < low_cost->total_cost && ((temp->cost_a > 0 && temp->cost_b > 0)
				|| (temp->cost_a < 0 && temp->cost_b < 0)))
			low_cost = temp;
		else if (temp->total_cost < low_cost->total_cost)
			low_cost = temp;
		if (temp->total_cost == low_cost->total_cost)
		{
			if (temp->number < low_cost->number)
				low_cost = temp;
		}	
		temp = temp->next;
	}
	return (low_cost);
}

t_node	*lowest_cost_b(t_node *stack_b)
{
	t_node	*low_cost;
	t_node	*temp;

	temp = stack_b;
	low_cost = stack_b;
	while (temp)
	{
		if (temp->total_cost < low_cost->total_cost && ((temp->cost_a > 0 && temp->cost_b > 0)
				|| (temp->cost_a < 0 && temp->cost_b < 0)))
			low_cost = temp;
		else if (temp->total_cost < low_cost->total_cost)
			low_cost = temp;
		/*if (temp->total_cost == low_cost->total_cost)
		{
			if (temp->number > low_cost->number)
				low_cost = temp;
		}*/	
		temp = temp->next;
	}
	return (low_cost);
}
