/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:57:21 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/25 12:51:12 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_putnstr(char *str, int len, t_printf *rules)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	if (rules->sign)
		rules->total_lenght += write(1, "-", 1);
	while (len > 0 && str[i] != '\0')
	{
		rules->total_lenght += write(1, &str[i], 1);
		i++;
		len--;
	}
}
