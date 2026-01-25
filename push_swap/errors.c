/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:48:26 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 17:47:10 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "push_swap.h"

int	duplicado(char **argumentos)
{
	int	i;
	int	j;

	i = 0;
	while (argumentos[i] != NULL)
	{
		j = i + 1;
		while (argumentos[j] != NULL)
		{
			if (strcmp(argumentos[i], argumentos[j]) == 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_errors(int quantidade, char **argumentos)
{
	int			i;
	long int	*b;

	if (ft_isnum(argumentos) == 0)
		return (0);
	i = 0;
	if (duplicado(argumentos) == 0)
		return (0);
	b = calloc((quantidade), sizeof(long int));
	while (argumentos[i] != NULL)
	{
		b[i] = atol(argumentos[i]);
		if (b[i] < INT_MIN || b[i] > INT_MAX)
			return (0);
		i++;
	}
	free (b);
	return (1);
}
