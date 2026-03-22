/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:33:49 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/31 21:34:06 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*aux;
	int		count;

	if (lst == NULL)
		return (NULL);
	aux = lst;
	while (aux != NULL)
	{
		count++;
		aux = aux->next;
	}
	return (count);
}
