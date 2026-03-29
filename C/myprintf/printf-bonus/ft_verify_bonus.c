/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_verify_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:14:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/25 12:51:18 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_verify(va_list ap, char s, t_printf *rules)
{
	if (ft_strchr("cpxX", s))
	{
		rules->type = 's';
		ft_convert_char(ap, s, rules);
	}
	else if (ft_strchr("diu%", s))
		ft_convert_num(rules, s);
	else if (s == 's')
		ft_convert_string(rules);
	else
	{
		write(1, "%", 1);
		write(1, &s, 1);
	}
}
