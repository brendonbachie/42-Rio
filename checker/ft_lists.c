/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lists.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:52:44 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 21:32:55 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*ft_lists(char **arguments)
{
	t_node	*head;
	t_node	*new;
	int		i;

	i = 0;
	head = NULL;
	while (arguments[i])
	{
		new = ft_lstnew(atoi(arguments[i]));
		if (!new)
		{
			ft_lstclear(&head);
			return (NULL);
		}
		ft_lstadd_back(&head, new);
		i++;
	}
	return (head);
}

int	has_duplicate(t_node *stack)
{
	t_node	*a;
	t_node	*b;

	a = stack;
	while (a)
	{
		b = a->next;
		while (b)
		{
			if (a->number == b->number)
				return (1);
			b = b->next;
		}
		a = a->next;
	}
	return (0);
}
