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

void	target(t_node *stack_a, t_node *stack_b)
{
	t_node	*temp_a;
	t_node	*temp_b;
	t_node	*target;
	long	best_value;

	temp_b = stack_b;
	while (temp_b)
	{
		target = NULL;
		best_value = 2147483649;
		temp_a = stack_a;
		while (temp_a)
		{
			if (temp_a->number > temp_b->number && temp_a->number < best_value)
			{
				best_value = temp_a->number;
				target = temp_a;
			}
			temp_a = temp_a->next;
		}
		if (!target)
			target = lowest_value(stack_a);
		temp_b->target = target;
		temp_b = temp_b->next;
	}
}

void	index_target(t_node *stack_a, t_node *stack_b)
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
				temp_a->target_index = i;
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

void	final_index(t_node *stack_a)
{
	t_node	*cur;
	t_node	*cmp;
	int		index;

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
