/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 22:13:47 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/25 12:50:39 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_right_cs(t_printf *tab)
{
	int	i;

	i = tab->width;
	if (tab->zero == 1)
		i--;
	if (tab->zero == 0)
	{
		while (i > 0)
		{
			tab->total_lenght += write(1, " ", 1);
			i--;
		}
	}
	if (tab->zero == 1 && tab->type != 's')
		tab->total_lenght += write(1, "-", 1);
	if (tab->hash == 1)
		i = i - 1;
	while (i > 0)
	{
		tab->total_lenght += write(1, "0", 1);
		i--;
	}
}

void	ft_right_cs_int(t_printf *tab, long num)
{
	int	i;

	if (num < 0 && tab->zero)
		tab->width--;
	i = tab->width;
	if (tab->zero == 0)
	{
		while (i > 0)
		{
			tab->total_lenght += write(1, " ", 1);
			i--;
		}
	}
	if (num < 0 && tab->dash == 0)
	{
		tab->total_lenght += write(1, "-", 1);
		tab->sign = 0;
	}
	if (tab->hash == 1)
		i = i - 1;
	while (i > 0)
	{
		tab->total_lenght += write(1, "0", 1);
		i--;
	}
}
