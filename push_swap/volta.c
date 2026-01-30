/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   volta.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 20:00:58 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 21:36:51 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	target_b(t_node *stack_a, t_node *stack_b)
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
