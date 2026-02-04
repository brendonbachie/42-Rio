/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turco.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:33:32 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 20:53:35 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	algorithm(t_node **stack_a, t_node **stack_b)
{
	final_index(*stack_a);
	if (ft_lstsize(stack_a) < 6)
	{
		while (ft_lstsize(stack_a) > 3)
			pb(stack_a, stack_b);
	}
	while (ft_lstsize(stack_a) > 5)
		send_to_b(stack_a, stack_b);
	if (ft_lstsize(stack_a) > 4)
		five(stack_a, stack_b);
	else
		three(stack_a);
	while (ft_lstsize(stack_b) > 0)
		b_to_a(stack_a, stack_b);
	last_move(stack_a);
}

void	b_to_a(t_node **stack_a, t_node **stack_b)
{
	t_node	*best_cost;

	index_node(*stack_b);
	target(*stack_a, *stack_b);
	index_target(*stack_b, *stack_a);
	cost(*stack_b, *stack_a);
	total_cost(*stack_b);
	best_cost = lowest_cost(*stack_b);
	chose_move(stack_b, stack_a, best_cost);
}

void	last_move(t_node **stack_a)
{
	t_node	*lowest;

	lowest = lowest_value(*stack_a);
	index_node(*stack_a);
	if (lowest->index_node <= ft_lstsize(stack_a) / 2)
	{
		while ((*stack_a)->number != lowest->number)
		{
			ra(stack_a);
			write(1, "ra\n", 3);
		}
	}
	else
	{
		while ((*stack_a)->number != lowest->number)
		{
			rra(stack_a);
			write(1, "rra\n", 4);
		}
	}
}

void	send_to_b(t_node **stack_a, t_node **stack_b)
{
	int	size;
	int	i;
	int	to_push;
	int	is_smaller;
	int	half;

	size = ft_lstsize(stack_a);
	is_smaller = 1;
	half = size / 2;
	to_push = size - 3;
	i = 0;
	while (i < size && to_push > 0)
	{
		while (!((*stack_a)->index <= half) && is_smaller)
		{
			ra(stack_a);
			write(1, "ra\n", 3);
		}
		pb(stack_a, stack_b);
		to_push--;
		if (to_push == half - 3)
			is_smaller = 0;
		i++;
	}
}
