/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:11:48 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/02 19:00:44 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

void	ra(t_node **stack_a)
{
	t_node	*temp;
	t_node	*first;

	if (stack_a == NULL || *stack_a == NULL || (*stack_a)->next == NULL)
		return ;
	first = *stack_a;
	*stack_a = (*stack_a)->next;
	first->next = NULL;
	temp = *stack_a;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = first;
}

void	rb(t_node **stack_b)
{
	t_node	*temp;
	t_node	*first;

	if (stack_b == NULL || *stack_b == NULL || (*stack_b)->next == NULL)
		return ;
	first = *stack_b;
	*stack_b = (*stack_b)->next;
	first->next = NULL;
	temp = *stack_b;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = first;
}

void	rr(t_node **stack_a, t_node **stack_b)
{
	ra(stack_a);
	rb(stack_b);
}
