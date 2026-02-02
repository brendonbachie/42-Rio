/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_string.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:16 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/01 20:58:52 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_convert_string(va_list ap, t_printf *rules)
{
	size_t	len;
	char	*s;

	s = va_arg(ap, char *);
	if (!s)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	ft_putstr_fd(s, 1);
	len = ft_strlen(s);
	return (len);
}
