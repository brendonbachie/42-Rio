/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendon <brendon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:47 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/16 10:07:30 by brendon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include <stdarg.h>

int	ft_printf(const char *s, ...)
{
	va_list	a;
	int		i;
	int		len;

	va_start(a, s);
	i = 0;

	len = 0;
	while (s[i])
	{
		if (s[i] == '%')
		{
			i++;
			if (ft_strchr("+ #", s[i]))
			{
				len += plus(a, s[i], s[i + 1]);
				i++;
			}
			else
				len += ft_verify(a, s[i]);
		}
		else
			len += (ft_putchar_fd(s[i], 1), 1);
		i++;
	}
	va_end(a);
	return (len);
}
