/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:57:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 17:57:49 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_elements	*ft_lstnew(int number)
{
	t_elements	*new;

	new = malloc(sizeof(t_elements));
	if (!new)
		return (NULL);
	new->number = number;
	new->next = NULL;
	return (new);
}
