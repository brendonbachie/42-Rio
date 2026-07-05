/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 00:30:00 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/07/05 00:30:00 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	start_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->last_compile_start = get_time();
	pthread_mutex_unlock(&coder->data->state_mutex);
	print_status(coder, " is compiling\n");
}

static void	finish_compiling(t_dongle *first, t_dongle *second, t_coder *coder)
{
	ft_usleep(coder->data->time_to_compile);
	release_dongle(second);
	release_dongle(first);
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->data->state_mutex);
}

static void	debug_and_refactor(t_coder *coder, t_data *data)
{
	if (simulation_over(data))
		return ;
	print_status(coder, " is debugging\n");
	ft_usleep(data->time_to_debug);
	if (simulation_over(data))
		return ;
	print_status(coder, " is refactoring\n");
	ft_usleep(data->time_to_refactor);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_data		*data;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)arg;
	data = coder->data;
	pick_order(coder, &first, &second);
	while (!simulation_over(data))
	{
		if (!acquire_dongle(first, coder))
			break ;
		if (!acquire_dongle(second, coder))
		{
			release_dongle(first);
			break ;
		}
		start_compiling(coder);
		finish_compiling(first, second, coder);
		debug_and_refactor(coder, data);
	}
	return (NULL);
}
