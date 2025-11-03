/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_num.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:00 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/03 16:07:02 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

int	ft_convert_num(va_list ap, char cvt)
{
	unsigned int	u;
	int				d;

	if (cvt == 'd' || cvt == 'i')
	{
		d = va_arg(ap, int);
		ft_putnbr_fd(d, 1);
		return (ft_numlen(d));
	}
	else if (cvt == 'u')
	{
		u = va_arg(ap, unsigned int);
		return (ft_put_uns_fd(u, 1));
	}
	else if (cvt == '%')
	{
		write(1, "%", 1);
		return (1);
	}
	return (0);
}
