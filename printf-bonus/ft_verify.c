/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_verify.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:14:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/01 20:57:40 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_verify(va_list ap, char s, t_printf *rules)
{
	int	len;

	len = 0;
	if (ft_strchr("cpxX", s))
		len += ft_convert_char(ap, s, rules);
	else if (ft_strchr("diu%", s))
		len += ft_convert_num(ap, s, rules);
	else if (s == 's')
		len += ft_convert_string(ap, rules);
	else
	{
		write(1, "%", 1);
		write(1, &s, 1);
		len += 2;
	}
	return (len);
}
