/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 00:30:00 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/07/05 00:30:00 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_ready(t_dongle *dongle, t_coder *coder)
{
	if (dongle->is_taken)
		return (0);
	if (get_time() < dongle->free_at)
		return (0);
	if (heap_peek_id(&dongle->queue) != coder->id)
		return (0);
	return (1);
}

void	build_timeout(t_dongle *dongle, struct timespec *ts)
{
	t_time	wake_at;

	wake_at = get_time() + 1;
	if (dongle->free_at > wake_at)
		wake_at = dongle->free_at;
	ts->tv_sec = wake_at / 1000;
	ts->tv_nsec = (wake_at % 1000) * 1000000;
}

t_request	build_request(t_coder *coder)
{
	t_request	req;

	req.id = coder->id;
	req.arrival_time = get_time();
	req.deadline = coder->last_compile_start + coder->data->time_to_burnout;
	return (req);
}
