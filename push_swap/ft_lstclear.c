/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:55:47 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 17:55:54 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_lstclear(t_elements **lst)
{
	t_elements	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
