/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_numbers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 19:07:48 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/02 19:07:48 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	small_numbers(t_node **stack_a)
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

void	five(t_node **stack_a, t_node **stack_b)
{
	while (ft_lstsize(stack_a) > 3)
	{
		if ((*stack_a)->number == greater_value(*stack_a)->number)
			pb(stack_a, stack_b);
		else
		{
			ra(stack_a);
			write(1, "ra\n", 3);
		}
	}
	three(stack_a);
	while (ft_lstsize(stack_a) < 5)
	{
		pa(stack_a, stack_b);
		ra(stack_a);
		write(1, "ra\n", 3);
	}
}
