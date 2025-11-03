/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:47 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/03 16:07:48 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include <stdarg.h>

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	int		i;
	int		len;

	i = 0;
	len = 0;
	if (!str)
		return (-1);
	va_start(ap, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%' && str[i + 1])
		{
			i++;
			if (ft_strchr("cpxdiusX%", str[i]))
				len += ft_verify(ap, str[i]);
		}
		else
			len += (ft_putchar_fd(str[i], 1), 1);
		i++;
	}
	va_end (ap);
	return (len);
}
