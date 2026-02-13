/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 22:12:28 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/12 22:12:37 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	start_stack(t_printf *head)
{
	head->width = 0;
	head->precision = -1;
	head->zero = 0;
	head->point = 0;
	head->hash = 0;
	head->total_lenght = 0;
	head->plus = 0;
	head->space = 0;
	head->dash = 0;
	head->sp = 0;
}

int	parse_format(const char *s, int i, t_printf *f)
{
	start_stack(f);
	while (ft_strchr("-0+ #", s[i]))
	{
		if (s[i] == '-')
			f->dash = 1;
		if (s[i] == '0' && f->dash == 0)
			f->zero = 1;
		if (s[i] == '+')
			f->plus = 1;
		if (s[i] == ' ')
			f->space = 1;
		if (s[i] == '#')
			f->hash = 1;
		i++;
	}
	i = parse_width(s, i, f);
	return (i);
}

int	parse_width(const char *s, int i, t_printf *f)
{
	if (s[i] >= '0' && s[i] <= '9')
	{
		f->width = 0;
		while (s[i] >= '0' && s[i] <= '9')
		{
			f->width = f->width * 10 + (s[i] - '0');
			i++;
		}
	}
	if (s[i] == '.')
	{
		i++;
		f->precision = 0;
		while (s[i] >= '0' && s[i] <= '9')
		{
			f->precision = f->precision * 10 + (s[i] - '0');
			i++;
		}
		f->zero = 0;
	}
	return (i);
}

void	put_zero(int zero, t_printf *rules, int num)
{
	if (num < 0)
		rules->total_lenght += write(1, "-", 1);
	while (zero > 0)
	{
		rules->total_lenght += write(1, "0", 1);
		zero--;
	}
}

void	ft_print_spaces_int(t_printf *rules, int len, int num)
{
	if (rules->hash == 1 && rules->zero == 0)
		rules->width = rules->width - 2;
	if (rules->space == 1)
		rules->width = rules->width - 1;
	if (len > rules->precision)
	{
		rules->width = rules->width - len;
		ft_right_cs_int(rules, num);
	}
	else
	{
		rules->width = rules->width - rules->precision;
		ft_right_cs_int(rules, num);
	}
}
