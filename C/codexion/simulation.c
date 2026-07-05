/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 00:20:00 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/07/05 00:30:00 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, char *status)
{
	t_time	current_time;

	pthread_mutex_lock(&coder->data->print_mutex);
	current_time = get_time() - coder->data->start_time;
	printf("%lld %ld%s", current_time, coder->id, status);
	pthread_mutex_unlock(&coder->data->print_mutex);
}

int	simulation_over(t_data *data)
{
	int	over;

	pthread_mutex_lock(&data->state_mutex);
	over = data->burnout_flag || data->completion_flag;
	pthread_mutex_unlock(&data->state_mutex);
	return (over);
}
