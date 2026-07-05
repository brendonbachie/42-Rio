/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 00:20:00 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/07/05 00:20:00 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_time	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((t_time)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(t_time time)
{
	t_time	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(500);
}

static void	assign_params(t_data *data, char **v)
{
	data->number_of_coders = atoi(v[1]);
	data->time_to_burnout = atoi(v[2]);
	data->time_to_compile = atoi(v[3]);
	data->time_to_debug = atoi(v[4]);
	data->time_to_refactor = atoi(v[5]);
	data->number_of_compiles_required = atoi(v[6]);
	data->dongle_cooldown = atoi(v[7]);
	if (strcmp(v[8], "edf") == 0)
		data->scheduler = EDF;
	else
		data->scheduler = FIFO;
}

t_data	*init_data(char **v)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	assign_params(data, v);
	data->burnout_flag = 0;
	data->completion_flag = 0;
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->state_mutex, NULL);
	data->start_time = get_time();
	data->dongle = init_dongles(data);
	data->coders = init_coders(data);
	if (!data->dongle || !data->coders)
	{
		printf("%s", "Memory allocation failed.\n");
		exit(1);
	}
	return (data);
}

void	free_data(t_data *data)
{
	long	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongle[i].mutex);
		pthread_cond_destroy(&data->dongle[i].cond);
		free(data->dongle[i].queue.requests);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	free(data->dongle);
	free(data->coders);
	free(data);
}
