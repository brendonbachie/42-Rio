/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:56:51 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/12 22:01:56 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_char(t_printf *tab)
{
	char	a;

	a = va_arg(tab->args, int);
	if (tab->width && tab->dash == 0)
	{
		tab->width = tab->width - 1;
		ft_right_cs(tab);
	}
	tab->total_lenght += write(1, &a, 1);
	if (tab->width && tab->dash)
	{
		tab->width = tab->width - 1;
		ft_right_cs(tab);
	}
}
