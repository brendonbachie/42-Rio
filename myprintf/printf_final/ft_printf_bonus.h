/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:08:25 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/27 16:22:18 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H

# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_printf
{
	va_list	args;
	int		width;
	int		precision;
	int		zero;
	int		point;
	int		hash;
	int		total_lenght;
	int		plus;
	int		space;
	int		dash;
	int		sp;
	char	type;
	int		sign;
}			t_printf;

int			ft_numlen(int n);
void		ft_convert_string(t_printf *rules);
void		ft_convert_char(va_list ap, char cvt, t_printf *rules);
void		ft_convert_num(t_printf *rules, char cvt);
int			ft_hex_fd(unsigned long n, int fd, char caps, t_printf *rules);
int			ft_printf(const char *str, ...);
int			ft_put_uns_fd(unsigned long n, int fd);
int			parse_format(const char *s, int i, t_printf *f);
void		ft_verify(va_list ap, char s, t_printf *rules);
void		ft_print_char(t_printf *tab);
void		ft_putnbr_fd(int n, int fd);
void		ft_putnstr(char *str, int len, t_printf *rules);
char		*ft_strchr(const char *s, int c);
size_t		ft_strlen(const char *s);
t_printf		*start_stack();
void		ft_right_cs(t_printf *tab);
void		restart_stack(t_printf *head);
void		ft_print_char(t_printf *tab);
void		nbr_flags(t_printf *rules, int num);
char		*ft_itoa(long n);
void		nbr_unsigned(t_printf *rules, long num);
void		nbr_hex(t_printf *rules, char cvt, int num);
void		ft_convert_hex(t_printf *rules, char cvt);
void		ft_convert_unsigned(t_printf *rules);
void		ft_print_spaces(t_printf *rules, int len);
int			parse_width(const char *s, int i, t_printf *f);
char		*ft_itoh(unsigned int n, char cvt);
void		nbr_pointer(t_printf *rules, char cvt, long num);
void		put_zero(int zero, t_printf *rules, long num);
void		ft_print_spaces_int(t_printf *rules, int len, long num);
void		ft_right_cs_int(t_printf *tab, long num);
void		ft_left_cs_int(t_printf *tab, long num);
void		ft_print_spaces_left(t_printf *rules, int len, long num);
void		hex_null(t_printf *rules);
#endif
