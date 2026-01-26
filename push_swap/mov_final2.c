/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mov_final2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:06:13 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 21:35:19 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	final_move_2(t_node **stack_b,
	t_node **stack_a, t_node *min_custo)
{
	int	cost_a;
	int	cost_b;

	cost_a = min_custo->cost_a;
	cost_b = min_custo->cost_b;
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
