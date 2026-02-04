/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:06:53 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/01 21:15:13 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int ft_convert_char(va_list ap, char cvt, t_printf *rules)
{
	void	*p;
	int		count;

	if (cvt == 'x' || cvt == 'X')
	{
		if (rules->dash && cvt == 'x')
			count += write (1, "0x", 2);
		else if (rules->dash && cvt == 'X')
			count += write(1, "0X", 2);
		count = ft_hex_fd(va_arg(ap, unsigned int), 1, cvt, rules);
		return (count);
	}
	else if (cvt == 'c')
	{
		ft_putchar_fd((char) va_arg(ap, int), 1);
		return (1);
	}
	else if (cvt == 'p')
	{
		p = va_arg(ap, void *);
		if (!p)
			return (write(1, "(nil)", 5));
		write(1, "0x", 2);
		count = ft_hex_fd((unsigned long)p, 1, 'p') + 2;
		return (count);
	}
	return (0);
}
