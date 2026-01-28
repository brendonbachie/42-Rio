/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:45:20 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 17:45:20 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*greater_value(t_node *stack)
{
	t_node	*temp;
	t_node	*greater;

	temp = stack;
	greater = stack;
	while (temp)
	{
		if (temp->number > greater->number)
			greater = temp;
		temp = temp->next;
	}
	return (greater);
}

void	target(t_node *stack_a, t_node *stack_b)
{
	long	best_value;
	t_node	*a;
	t_node	*b;
	t_node	*best;

	a = stack_a;
	while (a)
	{
		best = NULL;
		best_value = INT_MIN;
		b = stack_b;
		while (b)
		{
			if (b->number < a->number && b->number > best_value)
			{
				best_value = b->number;
				best = b;
			}
			b = b->next;
		}
		if (!best)
			best = greater_value(stack_b);
		a->target = best;
		a = a->next;
	}
}



void	index_b(t_node *stack_a, t_node *stack_b)
{
	int		i;
	t_node	*temp_a;
	t_node	*temp_b;

	temp_a = stack_a;
	while (temp_a)
	{
		i = 0;
		temp_b = stack_b;
		while (temp_b)
		{
			if (temp_a->target && temp_a->target == temp_b)
			{
				temp_a->index_b = i;
				break ;
			}
			temp_b = temp_b->next;
			i++;
		}
		temp_a = temp_a->next;
	}
}

void	index_node(t_node *stack)
{
	int		i;
	t_node	*temp;

	temp = stack;
	i = 0;
	while (temp)
	{
		temp->index_node = i;
		i++;
		temp = temp->next;
	}
}
