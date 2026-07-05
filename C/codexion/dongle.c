/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 23:35:55 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/07/05 00:30:00 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	pick_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

int	acquire_dongle(t_dongle *dongle, t_coder *coder)
{
	t_data			*data;
	t_request		req;
	struct timespec	ts;

	data = coder->data;
	req = build_request(coder);
	pthread_mutex_lock(&dongle->mutex);
	heap_push(&dongle->queue, req, data->scheduler);
	while (!dongle_ready(dongle, coder))
	{
		if (simulation_over(data))
		{
			heap_remove_id(&dongle->queue, coder->id, data->scheduler);
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		build_timeout(dongle, &ts);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
	}
	heap_pop(&dongle->queue, data->scheduler);
	dongle->is_taken = 1;
	pthread_mutex_unlock(&dongle->mutex);
	print_status(coder, " has taken a dongle\n");
	return (1);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_taken = 0;
	dongle->free_at = get_time() + dongle->cooldown;
	pthread_mutex_unlock(&dongle->mutex);
	pthread_cond_broadcast(&dongle->cond);
}
