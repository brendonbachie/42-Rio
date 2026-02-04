/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:23 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/01 21:11:27 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_hex_fd(unsigned long n, int fd, char caps, t_printf *tab)
{
	int	count;

	count = 0;
	if (n >= 16)
		count += ft_hex_fd(n / 16, fd, caps, tab);
	if (caps == 'X')
		write(fd, &"0123456789ABCDEF"[n % 16], 1);
	else
		write(fd, &"0123456789abcdef"[n % 16], 1);
	count += 1;
	return (count);
}
