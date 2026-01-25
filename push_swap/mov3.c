/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mov3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:13:13 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 18:14:47 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rra(t_elements **stack_a)
{
	t_elements	*temp;
	t_elements	*last;
	t_elements	*second_last;

	if (stack_a == NULL || *stack_a == NULL || (*stack_a)->next == NULL)
		return ;
	temp = *stack_a;
	while (temp->next != NULL)
	{
		second_last = temp;
		temp = temp->next;
	}
	last = temp;
	second_last->next = NULL;
	last->next = *stack_a;
	*stack_a = last;
}

void	rrb(t_elements **stack_b)
{
	t_elements	*temp;
	t_elements	*last;
	t_elements	*second_last;

	if (stack_b == NULL || *stack_b == NULL || (*stack_b)->next == NULL)
		return ;
	temp = *stack_b;
	while (temp->next != NULL)
	{
		second_last = temp;
		temp = temp->next;
	}
	last = temp;
	second_last->next = NULL;
	last->next = *stack_b;
	*stack_b = last;
}

void	rrr(t_elements **stack_a, t_elements **stack_b)
{
	rra(stack_a);
	rrb(stack_b);
}
