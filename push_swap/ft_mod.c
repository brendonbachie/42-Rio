/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mod.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:04:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 20:41:30 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_mod(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

t_node	*lowest_value(t_node *stack_a)
{
	t_node	*lowest;

	lowest = stack_a;
	while (stack_a != NULL)
	{
		if (stack_a->number < lowest->number)
			lowest = stack_a;
		stack_a = stack_a->next;
	}
	return (lowest);
}

void	free_matriz(char **str)
{
	int	j;

	j = 0;
	while (str[j] != NULL)
	{
		j++;
	}
	while (j >= 0)
	{
		free(str[j]);
		j--;
	}
	free (str);
}
