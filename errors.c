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

int main(int argc, char **argv)
{
	int i = 1;
	int a = 1;
	char 	*b;
	
	b = calloc(sizeof(char), argc);
/*	while (a < argc)
	{
		//if (argv[a][i] == '-')
		//	i++;
		if (!(isdigit(argv[a][i])))
			return (0);
		i++;
		a++;
	}
	*/i = 1;
	while (i < argc)
	{
		if (strchr(b, atoi(argv[i])) || !(atoi(argv[i]) > -2147483648 && atoi(argv[i]) < 2147483647))
			return (printf("%s", "Error"));
		b[i - 1] = atoi(argv[i]);
		i++;
	}
	i = 0;
	while (b)
	{
		printf("%d", b[i]);
		i++;
	}
	return (0);
}
