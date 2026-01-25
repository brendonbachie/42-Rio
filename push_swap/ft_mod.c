/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mod.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:04:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 18:05:10 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_mod(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

t_elements	*menor_valor(t_elements *stack_a)
{
	t_elements	*menor;

	menor = stack_a;
	while (stack_a != NULL)
	{
		if (stack_a->number < menor->number)
			menor = stack_a;
		stack_a = stack_a->next;
	}
	return (menor);
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
