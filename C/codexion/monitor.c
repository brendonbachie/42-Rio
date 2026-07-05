/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 00:07:08 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/07/05 00:09:26 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_data *data)
{
	long	i;
	t_time	now;

	i = 0;
	now = get_time();
	while (i < data->number_of_coders)
	{
		if (now - data->coders[i].last_compile_start > data->time_to_burnout)
		{
			print_status(&data->coders[i], " burned out\n");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_completion(t_data *data)
{
	long	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (data->coders[i].compiles_done < data->number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

static void	wake_all_dongles(t_data *data)
{
	long	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongle[i].mutex);
		pthread_cond_broadcast(&data->dongle[i].cond);
		pthread_mutex_unlock(&data->dongle[i].mutex);
		i++;
	}
}

void	*monitor_thread(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->state_mutex);
		if (check_burnout(data))
			data->burnout_flag = 1;
		else if (check_completion(data))
			data->completion_flag = 1;
		if (data->burnout_flag || data->completion_flag)
		{
			pthread_mutex_unlock(&data->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->state_mutex);
		usleep(500);
	}
	wake_all_dongles(data);
	return (NULL);
}
