/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:45:20 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 17:45:20 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_elements	*maior_valor(t_elements *stack)
{
	t_elements	*temp;
	t_elements	*maior;

	temp = stack;
	maior = stack;
	while (temp)
	{
		if (temp->number > maior->number)
			maior = temp;
		temp = temp->next;
	}
	return (maior);
}

void	alvo(t_elements *stack_a, t_elements *stack_b)
{
	long		best_value;
	t_elements	*a;
	t_elements	*b;
	t_elements	*best;

	a = stack_a;
	while (a)
	{
		best = NULL;
		best_value = -2147483649;
		b = stack_b;
		while (b)
		{
			if (b->number < a->number && b->number > best_value)
			{
				best_value = b->number;
				best = b;
			}
			b = b->next;
		}
		if (!best)
			best = maior_valor(stack_b);
		a->alvo = best;
		a = a->next;
	}
}

void	index_b(t_elements *stack_a, t_elements *stack_b)
{
	int			i;
	t_elements	*temp_a;
	t_elements	*temp_b;

	temp_a = stack_a;
	while (temp_a)
	{
		i = 0;
		temp_b = stack_b;
		while (temp_b)
		{
			if (temp_a->alvo && temp_a->alvo == temp_b)
			{
				temp_a->index_b = i;
				break ;
			}
			temp_b = temp_b->next;
			i++;
		}
		temp_a = temp_a->next;
	}
}

void	indice(t_elements *stack)
{
	int			i;
	t_elements	*temp;

	temp = stack;
	i = 0;
	while (temp)
	{
		temp->index = i;
		i++;
		temp = temp->next;
	}
}
