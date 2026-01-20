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
	int i = 1;
	char 	*b;
	
	b = calloc(sizeof(char), argc);
	while (i < argc)
	{
		if (argv[i] == '-')
			i++;
		if (!(isdigit(argv[i])))
		{
			printf("%s", "Error");
			return (0);
		}
		i++;
	}
	i = 0;
	while (i < argc)
	{
		if (strchr(b, atoi(argv)) || !(atoi(argv) > -2147483648 && atoi(argv) < 2147483647))
			return (printf("%s", "Error"));
		b[i - 1] = atoi(argv);
		i++;
	}
	i = 0;
	while (b[i] != '\0')
	{
		printf("%d\n", b[i]);
		i++;
	}
	return (0);
}
