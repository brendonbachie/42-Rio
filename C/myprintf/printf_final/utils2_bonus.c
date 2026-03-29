/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:36:56 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/27 16:21:42 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_print_spaces_left(t_printf *rules, int len, long num)
{
	if (rules->hash == 1 && rules->zero == 0)
		rules->width = rules->width - 2;
	if (rules->space == 1)
		rules->width = rules->width - 1;
	if (len > rules->precision)
	{
		rules->width = rules->width - len;
		ft_left_cs_int(rules, num);
	}
	else
	{
		rules->width = rules->width - rules->precision;
		ft_left_cs_int(rules, num);
	}
}

void	ft_left_cs_int(t_printf *tab, long num)
{
	int	i;

	(void)num;
	i = tab->width;
	if (tab->zero == 0)
	{
		while (i > 0)
		{
			tab->total_lenght += write(1, " ", 1);
			i--;
		}
	}
	
}

void	restart_stack(t_printf *head)
{

	head->width = 0;
	head->precision = -1;
	head->zero = 0;
	head->point = 0;
	head->hash = 0;
	head->plus = 0;
	head->space = 0;
	head->dash = 0;
	head->sp = 0;
	head->sign = 0;
}