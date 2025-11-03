#ifndef LIBFTPRINTF_H
#define LIBFTPRINTF_H

#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "libft.h"
#include <stdarg.h>

int	ft_convert_char(va_list ap, char cvt);
int	ft_convert_num(va_list ap, char cvt);
int	ft_hex_fd(unsigned long n, int fd, char caps);
int	ft_numlen(int n);
int	ft_printf(const char *str , ...);
int	ft_put_uns_fd(unsigned int n, int fd);
int	ft_convert_string(va_list ap);

#endif
