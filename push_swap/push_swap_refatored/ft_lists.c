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
		new = ft_lstnew(ft_atoi(arguments[i]));
		if (!new)
		{
			ft_lstclear(&head);
			free_matriz(arguments);
			return (NULL);
		}
		ft_lstadd_back(&head, new);
		i++;
	}
	return (head);
}

void ft_lstadd_back(t_node **lst, t_node *new)
{
	t_node *tmp;

	if (!lst || !new)
		return;
	if (!*lst)
	{
		*lst = new;
		return;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void ft_lstclear(t_node **lst)
{
	t_node *tmp;

	if (!lst)
		return;
	while (*lst)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

t_node *ft_lstnew(int number)
{
	t_node *new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->number = number;
	new->next = NULL;
	return (new);
}

int ft_lstsize(t_node **lst)
{
	t_node *temp;
	int size;

	size = 0;
	temp = *lst;
	while (temp != NULL)
	{
		temp = temp->next;
		size++;
	}
	return (size);
}
