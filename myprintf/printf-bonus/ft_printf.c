/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:47 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/01 21:31:46 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *s, ...)
{
	va_list	a;
	int		i;
	int		len;
	t_printf	*rules;

	va_start(a, s);
	i = 0;
	len = 0;
	rules = malloc (sizeof(t_printf));
	start_stack(rules);
	if (!s)
		return (-1);
	while (s[i])
	{
		if (s[i] == '%' && s[i + 1])
		{
			i++;
			i = parse_format(s, i, rules);
			len += ft_verify(a, s[i], rules);
		}
		else
			len += (ft_putchar_fd(s[i], 1), 1);
		i++;
	}










































			

		{
			i++;
			if (ft_strchr("uidscxXp%", s[i]))
				len += ft_verify(a, s[i]);
			else
				len += (ft_putchar_fd(s[i], 1), 1);
		}
		else
			len += (ft_putchar_fd(s[i], 1), 1);
		i++;
	}
	va_end(a);
	return (len);
}
