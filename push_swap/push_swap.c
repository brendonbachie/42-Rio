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
	splitandjoin = ft_splitandjoin(argc, argv);
	head = ft_lists(splitandjoin);
	if (ft_errors(argc, splitandjoin) == 0
		|| has_duplicate(head) == 1)
	{
		free_matriz(splitandjoin);
		write(2, "Error\n", 6);
		return (-1);
	}
	if (is_ordened(head))
		return (0);
	if (ft_lstsize(&head) <= 3)
		to_three(&head);
	else
		turkey(&head, &head2);
	free_matriz(splitandjoin);
	ft_lstclear(&head);
	ft_lstclear(&head2);
	return (0);
}

int	is_ordened(t_node *stack_a)
{
	t_node	*temp;

	temp = stack_a;
	while(temp->next)
	{
		if (temp->number < temp->next->number)
			temp = temp->next;
		else
			return (0);
	}
	return (1);
}