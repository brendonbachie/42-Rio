/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:06:53 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/03 16:06:54 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

int	ft_convert_char(va_list ap, char cvt)
{
	void	*p;

	if (cvt == 'x' || cvt == 'X')
		return (ft_hex_fd(va_arg(ap, unsigned int), 1, cvt));
	else if (cvt == 'c')
	{
		ft_putchar_fd((char) va_arg(ap, int), 1);
		return (1);
	}
	else if (cvt == 'p')
	{
		p = va_arg(ap, void *);
		write(1, "0x", 2);
		ft_hex_fd((unsigned long)p, 1, 'p');
		return (14);
	}
	return (0);
}
