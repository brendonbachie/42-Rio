/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_num.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:00 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/12 22:08:20 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_convert_num(t_printf *rules, char cvt)
{
	int	d;

	if (cvt == 'd' || cvt == 'i')
	{
		d = va_arg(rules->args, int);
		if (d == 0 && (rules->precision == 0))
			return ;
		nbr_flags(rules, d);
	}
	else if (cvt == 'u')
		ft_convert_unsigned(rules);
	else if (cvt == '%')
	{
		rules->total_lenght += write(1, "%", 1);
		return ;
	}
	return ;
}

void	ft_convert_unsigned(t_printf *rules)
{
	unsigned int	u;
	int				d;

	d = va_arg(rules->args, int);
	if (d < 0)
	{
		u = va_arg(rules->args, unsigned int);
		rules->total_lenght += ft_put_uns_fd(u, 1);
	}
	else
		nbr_flags(rules, d);
}
