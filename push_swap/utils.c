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

t_node	*lowest_value(t_node *stack_a)
{
	t_node	*lowest;
	t_node	*temp;

	lowest = stack_a;
	temp = stack_a;
	while (temp != NULL)
	{
		if (temp->number < lowest->number)
			lowest = temp;
		temp = temp->next;
	}
	return (lowest);
}

t_node	*greater_value(t_node *stack)
{
	t_node	*temp;
	t_node	*greater;

	temp = stack;
	greater = stack;
	while (temp)
	{
		if (temp->number > greater->number)
			greater = temp;
		temp = temp->next;
	}
	return (greater);
}

int	args_vrf(char **argv, int argc)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	while (i < argc)
	{
		j = ft_strlen(argv[i]);
		k = 0;
		while (k <= j)
		{
			if (argv[i][0] == '\0')
			{
				write (2, "Error\n", 6);
				return (1);
			}
			k++;
		}
		i++;
	}
	return (0);
}
