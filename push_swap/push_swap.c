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
	head = ft_lists(splitandjoin);
	if (!head || ft_errors(ft_lstsize(&head), splitandjoin, head))
	{
		return_error(&head, splitandjoin);
		return (-1);
	}
	if (ft_lstsize(&head) <= 3)
		few_numbers(&head);
	else
		turkey(&head, &head2);
	free_matrix(splitandjoin);
	ft_lstclear(&head);
	ft_lstclear(&head2);
	return (0);
}

