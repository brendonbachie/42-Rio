/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_min_custo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:07:06 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 19:07:13 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_elements	*push_min_custo(t_elements *stack_a)
{
	t_elements	*min_custo;
	t_elements	*temp;

	temp = stack_a;
	min_custo = stack_a;
	while (temp)
	{
		if (temp->custo_total < min_custo->custo_total)
			min_custo = temp;
		temp = temp->next;
	}
	return (min_custo);
}
