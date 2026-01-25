/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:01:15 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 18:01:19 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_lstsize(t_elements **lst)
{
	t_elements	*temp;
	int			size;

	size = 0;
	temp = *lst;
	while (temp != NULL)
	{
		temp = temp->next;
		size++;
	}
	return (size);
}
