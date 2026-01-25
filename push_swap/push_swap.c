/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:07:48 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 19:09:23 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "push_swap.h"

int	main(int argc, char **argv)
{
	char		**splitejoin;
	t_elements	*head;
	t_elements	*head2;

	head = NULL;
	head2 = NULL;
	splitejoin = ft_splitejoin(argc, argv);
	if (ft_errors(argc, splitejoin) == 0)
	{
		free(splitejoin);
		printf("%s", "Error");
		return (-1);
	}
	head = ft_lists(splitejoin);
	if (ft_lstsize(&head) <= 3)
		ate3(&head);
	else
		turco(&head, &head2);
	free_matriz(splitejoin);
	ft_lstclear(&head);
	ft_lstclear(&head2);
	return (0);
}
