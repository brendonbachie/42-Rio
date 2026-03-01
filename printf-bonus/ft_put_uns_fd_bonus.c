/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_uns_fd_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:53 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/25 12:51:06 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_put_uns_fd(unsigned long n, int fd)
{
	int	count;

	count = 0;
	if (n >= 10)
		count += ft_put_uns_fd(n / 10, fd);
	write(fd, &"0123456789"[n % 10], 1);
	count += 1;
	return (count);
}
