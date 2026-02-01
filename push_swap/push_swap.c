/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:07:48 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/26 06:34:43 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "push_swap.h"

int	main(int argc, char **argv)
{
	char	**splitandjoin;
	t_node	*head;
	t_node	*head2;

	head = NULL;
	head2 = NULL;
	if (argc < 2)
		return (-1);
	splitandjoin = ft_splitandjoin(argv);
	if (!splitandjoin || !splitandjoin[0])
	{
		return_error(&head, splitandjoin);
		write (2, "Error\n", 6);
		return (0);
	}
	head = ft_lists(splitandjoin);
	if (!head || ft_errors(splitandjoin, head))
	{
		return_error(&head, splitandjoin);
		return (0);
	}
	push_swap(&head, &head2);
	free_matrix(splitandjoin);
	return (0);
}

void	push_swap(t_node **head, t_node **head2)
{
	if (ft_lstsize(head) <= 3)
		small_numbers(head);
	else
		algorithm(head, head2);
	ft_lstclear(head);
	ft_lstclear(head2);
}

void	free_matrix(char **str)
{
	int	j;

	j = 0;
	while (str[j] != NULL)
		j++;
	while (j >= 0)
	{
		free(str[j]);
		j--;
	}
	free(str);
}
