/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_verify.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:14:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/03 16:14:26 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.a"
#include "libftprintf.a"

int	ft_verify(va_list ap, char s)
{
	int	len;

	len = 0;
	if (ft_strchr("cpxX", s))
		len += ft_convert_char(ap, s);
	else if (ft_strchr("diu%", s))
		len += ft_convert_num(ap, s);
	else if (s == 's')
		len += ft_convert_string(ap);
	return (len);
}
