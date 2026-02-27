/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:47 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/25 13:08:44 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_printf(const char *s, ...)
{
	int			i;
	t_printf	*rules;
	va_list		a;

	i = 0;
	rules = malloc(sizeof(t_printf));
	start_stack(rules);
	rules->total_lenght = 0;
	if (!s)
		return (-1);
	va_start(rules->args, s);
	while (s[i])
	{
		if (s[i] == '%' && s[i + 1])
		{
			i++;
			i = parse_format(s, i, rules);
			ft_verify(rules->args, s[i++], rules);
		}
		else
			rules->total_lenght += write(1, &s[i++], 1);
	}
	va_end(a);
	return (rules->total_lenght);
}
