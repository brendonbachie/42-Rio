/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_num_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:00 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/25 12:34:58 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_convert_num(t_printf *rules, char cvt)
{
	int	d;

	if (cvt == 'd' || cvt == 'i')
	{
		d = va_arg(rules->args, int);
		if (d == 0 && (rules->precision == 0) && !rules->width)
			return ;
		if (d == 0 && (rules->precision == 0) && rules->width)
		{
			while (rules->width)
			{
				rules->total_lenght += write(1, " ", 1);
				rules->width--;
			}
			return ;
		}
		nbr_flags(rules, d);
	}
	else if (cvt == 'u')
		ft_convert_unsigned(rules);
	else if (cvt == '%')
		rules->total_lenght += write(1, "%", 1);
}

void	ft_convert_unsigned(t_printf *rules)
{
	unsigned int	u;
	long			d;

	d = va_arg(rules->args, long);
	if (d < 0)
	{
		u = va_arg(rules->args, unsigned long);
		rules->total_lenght += ft_put_uns_fd(u, 1);
	}
	else
		nbr_unsigned(rules, d);
}
