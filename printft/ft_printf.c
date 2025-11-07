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

int	ft_printf(const char *s, ...)
{
	va_list	a;
	int		i;
	int		len;

	if (!s)
		return (-1);
	va_start(a, s);
	i = 0;
	len = 0;
	while (s[i])
	{
		if (s[i] == '%' && s[i + 1])
		{
			i++;
			len += ft_verify(a, s[i]);
		}
		else
			len += (ft_putchar_fd(s[i], 1), 1);
		i++;
	}
	va_end(a);
	return (len);
}

