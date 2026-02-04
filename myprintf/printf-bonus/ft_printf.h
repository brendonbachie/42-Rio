/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:08:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/01 21:27:53 by bgomes-b         ###   ########.fr       */
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

typedef struct s_printf
{
	va_list args;
	int width;
	int precision;
	int zero;
	int point;
	int hash;
	int total_lenght;
	int plus;
	int space;
	int dash;
	int sp;
}	t_printf;

int		ft_convert_char(va_list ap, char cvt, t_printf *rules);
int ft_convert_num(va_list ap, char cvt, t_printf *rules);
int ft_hex_fd(unsigned long n, int fd, char caps, t_printf *rules);
int		ft_numlen(int n);
int		ft_printf(const char *str, ...);
int		ft_put_uns_fd(unsigned int n, int fd);
int ft_convert_string(va_list ap, t_printf *rules);
int ft_verify(va_list ap, char s, t_printf *rules);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
void start_stack(t_printf *head);
int parse_format(const char *s, int i, t_printf *f);

#endif
