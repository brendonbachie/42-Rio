/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_push_swap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:05:50 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 20:36:44 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	to_three(t_node **stack_a)
{
	int	size;

	size = ft_lstsize(stack_a);
	if (size <= 1)
		return ;
	if (size == 2)
	{
		if ((*stack_a)->number > (*stack_a)->next->number)
			sa(stack_a);
		return ;
	}
	if (size == 3)
	{
		three(stack_a);
		return ;
	}
}
