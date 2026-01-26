/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:48:26 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 20:38:04 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "push_swap.h"

int	duplicated(char **arguments)
{
	int	i;
	int	j;

	i = 0;
	while (arguments[i] != NULL)
	{
		j = i + 1;
		while (arguments[j] != NULL)
		{
			if (strcmp(arguments[i], arguments[j]) == 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_errors(int arg_count, char **arguments)
{
	int			i;
	long int	*b;

	if (ft_isnum(arguments) == 0)
		return (0);
	i = 0;
	if (duplicated(arguments) == 0)
		return (0);
	b = calloc((arg_count), sizeof(long int));
	while (arguments[i] != NULL)
	{
		b[i] = atol(arguments[i]);
		if (b[i] < INT_MIN || b[i] > INT_MAX)
			return (0);
		i++;
	}
	free (b);
	return (1);
}
