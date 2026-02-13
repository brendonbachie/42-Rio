/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:06:53 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/12 22:07:01 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_convert_char(va_list ap, char cvt, t_printf *rules)
{
	void	*p;
	int		count;

	(void)ap;
	count = 0;
	if (cvt == 'x' || cvt == 'X')
	{
		ft_convert_hex(rules, cvt);
	}
	else if (cvt == 'c')
	{
		ft_print_char(rules);
	}
	else if (cvt == 'p')
	{
		p = va_arg(rules->args, void *);
		if (!p)
			rules->total_lenght += write(1, "(nil)", 5);
		nbr_pointer(rules, cvt, (long)p);
	}
}

void	ft_convert_hex(t_printf *rules, char cvt)
{
	int	count;
	int	d;

	count = 0;
	if (cvt == 'x' || cvt == 'X')
	{
		d = va_arg(rules->args, int);
		if (d == 0 && (rules->hash == 1))
		{
			rules->total_lenght += write(1, "0", 1);
			return ;
		}
		nbr_hex(rules, cvt, d);
	}
}
