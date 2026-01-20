/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendon <brendon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:08:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/16 09:45:52 by brendon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stddef.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdarg.h>

int		ft_convert_char(va_list ap, char cvt);
int		ft_convert_num(va_list ap, char cvt);
int		ft_hex_fd(unsigned long n, int fd, char caps);
int		ft_numlen(int n);
int		ft_printf(const char *str, ...);
int		ft_put_uns_fd(unsigned int n, int fd);
int		ft_convert_string(va_list ap);
int		ft_verify(va_list ap, char s);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);

#endif
