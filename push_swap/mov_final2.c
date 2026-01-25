/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mov_final2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:06:13 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 19:06:54 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	mov_final2(t_elements **stack_b,
	t_elements **stack_a, t_elements *min_custo)
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
		mov_b(stack_b, custo_a);
	if (custo_b > 0 || custo_b < 0)
		mov_a(stack_a, custo_b);
	pa(stack_a, stack_b);
}
