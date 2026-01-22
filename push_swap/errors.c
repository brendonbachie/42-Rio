/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:48:26 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/20 11:48:27 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int ft_errors(int argc, char *argv)
{
	int i = 0;
	char 	*b;

	while (argv[i] != '\0')
	{
		if (argv[i] != '-' || argv[i] != '+' || !(argv[i] > '0' && argv[i] < '9'))
			return (0);
		i++;
	}
	b = calloc(sizeof(char), argc);
	i = 0;
	while (i < argc)
	{
		if (strchr(b, atoi(argv)) || !(atoi(argv) > -2147483648 && atoi(argv) < 2147483647))
			return (0);
		b[0] = atoi(argv);
		i++;
	}
	free (b);
	return (1);
}