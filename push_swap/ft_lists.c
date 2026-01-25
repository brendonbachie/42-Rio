/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lists.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:52:44 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 17:54:29 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_elements	*ft_lists(char **argumentos)
{
	t_elements	*head;
	t_elements	*new;
	int			i;

	i = 0;
	head = NULL;
	while (argumentos[i])
	{
		new = ft_lstnew(atoi(argumentos[i]));
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
