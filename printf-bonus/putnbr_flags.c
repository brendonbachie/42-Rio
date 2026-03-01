/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putnbr_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 22:14:21 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/13 04:33:33 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	nbr_flags(t_printf *rules, int num)
{
	int		len;
	int		zero;
	char	*number;

	if(num > 0)
		rules->width = rules->width - rules->plus;
	number = ft_itoa(num);
	len = ft_strlen(number);
	if (num < 0)
		rules->sign = 1;
	if (rules->space && num >= 0)
		rules->total_lenght += write(1, " ", 1);
	if (num > 0 && rules->plus)
		rules->total_lenght += write(1, "+", 1);
	if (num < 0 && rules->zero == 0)
		rules->width--;
	if (rules->width > 0 && rules->dash == 0)
		ft_print_spaces_int(rules, len, num);
	zero = rules->precision - len;
	put_zero(zero, rules, num);
	len = ft_strlen(number);
	ft_putnstr(number, len, rules);
	if (rules->width > 0 && rules->dash)
		ft_print_spaces_left(rules, len, num);
	free(number);
}

void	nbr_unsigned(t_printf *rules, long num)
{
	int		len;
	int		zero;
	char	*number;

	rules->width = rules->width - rules->plus;
	number = ft_itoa(num);
	len = ft_strlen(number);
	if (rules->space && num >= 0)
		rules->total_lenght = write(1, " ", 1);
	if (num > 0 && rules->plus)
		rules->total_lenght = write(1, "+", 1);
	if (num < 0 && rules->zero == 0)
		rules->width--;
	if (rules->width > 0 && rules->dash == 0)
		ft_print_spaces_int(rules, len, num);
	zero = rules->precision - len;
	put_zero(zero, rules, num);
	len = ft_strlen(number);
	ft_putnstr(number, len, rules);
	if (rules->width > 0 && rules->dash)
		ft_print_spaces_left(rules, len, num);
	free(number);
}

void	nbr_hex(t_printf *rules, char cvt, int num)
{
	int		len;
	int		zero;
	char	*number;

	number = ft_itoh(num, cvt);
	len = ft_strlen(number);
	if (rules->width && rules->dash == 0
		&& rules->hash == 1 && rules->zero == 0)
		ft_print_spaces(rules, len);
	if (rules->hash && cvt == 'x')
		rules->total_lenght += write(1, "0x", 2);
	else if (rules->hash && cvt == 'X')
		rules->total_lenght += write(1, "0X", 2);
	if (rules->width && rules->dash == 0)
		ft_print_spaces(rules, len);
	zero = rules->precision - len;
	put_zero(zero, rules, 1);
	len = ft_strlen(number);
	rules->total_lenght += ft_hex_fd(num, 1, cvt, rules);
	if (rules->width && rules->dash)
		ft_print_spaces(rules, len);
	free(number);
}

void	ft_print_spaces(t_printf *rules, int len)
{
	if (rules->hash == 1 && rules->zero == 0)
		rules->width = rules->width - 2;
	if (rules->space == 1)
		rules->width = rules->width - 1;
	if (len > rules->precision)
	{
		rules->width = rules->width - len;
		ft_right_cs(rules);
	}
	else
	{
		rules->width = rules->width - rules->precision;
		ft_right_cs(rules);
	}
}

void	nbr_pointer(t_printf *rules, char cvt, long num)
{
	int		len;
	int		zero;
	char	*number;

	rules->width -= 2;
	number = ft_itoh(num, cvt);
	len = ft_strlen(number);
	rules->hash = 0;
	if (rules->width && rules->dash == 0)
		ft_print_spaces(rules, len);
	zero = rules->precision - len;
	put_zero(zero, rules, num);
	len = ft_strlen(number);
	rules->total_lenght += write(1, "0x", 2);
	rules->total_lenght += ft_hex_fd(num, 1, cvt, rules);
	if (rules->width && rules->dash)
		ft_print_spaces(rules, len);
	free(number);
}
