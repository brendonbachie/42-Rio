/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mov_final.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:15:37 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 19:02:34 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	mov_final(t_elements **stack_a,
	t_elements **stack_b, t_elements *min_custo)
{
	int	custo_a;
	int	custo_b;

	custo_a = min_custo->custo_a;
	custo_b = min_custo->custo_b;
	while (custo_a > 0 && custo_b > 0)
	{
		rr(stack_a, stack_b);
		custo_a--;
		custo_b--;
		write(1, "rr\n", 3);
	}
	while (custo_a < 0 && custo_b < 0)
	{
		rrr(stack_a, stack_b);
		custo_a++;
		custo_b++;
		write(1, "rrr\n", 4);
	}
	if (custo_a > 0 || custo_a < 0)
		mov_a(stack_a, custo_a);
	if (custo_b > 0 || custo_b < 0)
		mov_b(stack_b, custo_b);
	pb(stack_a, stack_b);
}

void	mov_a(t_elements **stack, int custo)
{
	while (custo > 0)
	{
		ra(stack);
		write(1, "ra\n", 3);
		custo--;
	}
	while (custo < 0)
	{
		rra(stack);
		write(1, "rra\n", 4);
		custo++;
	}
}

void	mov_b(t_elements **stack, int custo)
{
	while (custo > 0)
	{
		rb(stack);
		write(1, "rb\n", 3);
		custo--;
	}
	while (custo < 0)
	{
		rrb(stack);
		write(1, "rrb\n", 4);
		custo++;
	}
}
