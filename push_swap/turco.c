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

void	turkey(t_node **stack_a, t_node **stack_b)
{
	
	if (ft_lstsize(stack_a) > 3)
		pb(stack_a, stack_b);
	while (ft_lstsize(stack_a) > 3)
		a_to_b(stack_a, stack_b);
	three(stack_a);
	last_move_b(stack_b);
	while (ft_lstsize(stack_b) > 0)
		b_to_a(stack_a, stack_b);
	last_move(stack_a);
}

void	a_to_b(t_node **stack_a, t_node **stack_b)
{
	t_node	*best_cost;

	index_node(*stack_a);
	target(*stack_a, *stack_b);
	index_b(*stack_a, *stack_b);
	cost(*stack_a, *stack_b);
	total_cost(*stack_a);
	best_cost = lowest_cost_a(*stack_a);
	final_move(stack_a, stack_b, best_cost);
}

void	b_to_a(t_node **stack_a, t_node **stack_b)
{
	t_node	*best_cost;

	index_node(*stack_b);
	target_b(*stack_a, *stack_b);
	index_b(*stack_b, *stack_a);
	cost(*stack_b, *stack_a);
	total_cost(*stack_b);
	best_cost = lowest_cost_b(*stack_b);
	final_move_2(stack_b, stack_a, best_cost);
}

void	last_move(t_node **stack_a)
{
	t_node	*menor;

	menor = lowest_value(*stack_a);
	index_node(*stack_a);
	if (menor->index_node <= ft_lstsize(stack_a) / 2)
	{
		while ((*stack_a)->number != menor->number)
		{
			ra(stack_a);
			write(1, "ra\n", 3);
		}
	}
	else
	{
		while ((*stack_a)->number != menor->number)
		{
			rra(stack_a);
			write(1, "rra\n", 4);
		}
	}
}

void	last_move_b (t_node **stack_b)
{
	t_node	*menor;

	menor = lowest_value(*stack_b);
	index_node(*stack_b);
	if (menor->index_node <= ft_lstsize(stack_b) / 2)
	{
		while ((*stack_b)->number != menor->number)
		{
			rb(stack_b);
			write(1, "rb\n", 3);
		}
	}
	else
	{
		while ((*stack_b)->number != menor->number)
		{
			rrb(stack_b);
			write(1, "rrb\n", 4);
		}
	}
}