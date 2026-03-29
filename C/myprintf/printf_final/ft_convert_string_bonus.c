/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_string_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:16 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/25 12:35:01 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_convert_string(t_printf *rules)
{
	int		len;
	char	*s;

	s = va_arg(rules->args, char *);
	if (!s)
	{
		rules->total_lenght += write(1, "(null)", 6);
		return ;
	}
	len = ft_strlen(s);
	if (rules->precision >= 0 && rules->precision < len)
		len = rules->precision;
	if (rules->width && rules->dash == 0)
	{
		rules->width = rules->width - len;
		ft_right_cs(rules);
	}
	ft_putnstr(s, len, rules);
	if (rules->width && rules->dash)
	{
		rules->width = rules->width - len;
		ft_right_cs(rules);
	}
}
