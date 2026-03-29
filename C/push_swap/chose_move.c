/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mov_final.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:15:37 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 21:35:08 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	chose_move(t_node **stack_b, t_node **stack_a, t_node *lowest_cost)
{
	int	cost_a;
	int	cost_b;

	cost_a = lowest_cost->cost_a;
	cost_b = lowest_cost->cost_b;
	while (cost_a > 0 && cost_b > 0)
	{
		rr(stack_a, stack_b);
		cost_a--;
		cost_b--;
		write(1, "rr\n", 3);
	}
	while (cost_a < 0 && cost_b < 0)
	{
		rrr(stack_a, stack_b);
		cost_a++;
		cost_b++;
		write(1, "rrr\n", 4);
	}
	if (cost_a > 0 || cost_a < 0)
		mov_b(stack_b, cost_a);
	if (cost_b > 0 || cost_b < 0)
		mov_a(stack_a, cost_b);
	pa(stack_a, stack_b);
}

void	mov_a(t_node **stack, int cost)
{
	while (cost > 0)
	{
		ra(stack);
		write(1, "ra\n", 3);
		cost--;
	}
	while (cost < 0)
	{
		rra(stack);
		write(1, "rra\n", 4);
		cost++;
	}
}

void	mov_b(t_node **stack, int cost)
{
	while (cost > 0)
	{
		rb(stack);
		write(1, "rb\n", 3);
		cost--;
	}
	while (cost < 0)
	{
		rrb(stack);
		write(1, "rrb\n", 4);
		cost++;
	}
}
