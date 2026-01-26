/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   three.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:32:16 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 20:35:31 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	three(t_node **stack_a)
{
	if ((*stack_a)->number > (*stack_a)->next->number)
	{
		if ((*stack_a)->number > (*stack_a)->next->next->number)
		{
			ra(stack_a);
			write(1, "ra\n", 3);
			if ((*stack_a)->number > (*stack_a)->next->number)
				sa(stack_a);
		}
		else
			sa(stack_a);
	}
	else if ((*stack_a)->next->number > (*stack_a)->next->next->number)
	{
		rra(stack_a);
		write(1, "rra\n", 4);
		if ((*stack_a)->number > (*stack_a)->next->number)
			sa(stack_a);
	}
}
