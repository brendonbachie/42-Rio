/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 23:59:05 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/07/05 00:00:12 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	comp_higher_priority(t_request a, t_request b, t_scheduler sched)
{
	if (sched == FIFO)
	{
		if (a.arrival_time != b.arrival_time)
			return (a.arrival_time < b.arrival_time);
		return (a.id < b.id);
	}
	if (a.deadline != b.deadline)
		return (a.deadline < b.deadline);
	return (a.id > b.id);
}

long	heap_peek_id(t_heap *heap)
{
	if (heap->size == 0)
		return (-1);
	return (heap->requests[0].id);
}

void	heap_remove_id(t_heap *heap, long id, t_scheduler sched)
{
	int	i;

	i = 0;
	while (i < heap->size && heap->requests[i].id != id)
		i++;
	if (i == heap->size)
		return ;
	heap->requests[i] = heap->requests[heap->size - 1];
	heap->size--;
	heap_sift_down(heap, i, sched);
}
