/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendon <brendon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:07:47 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/16 10:07:30 by brendon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct	s_flag
{
	char		*content;
}			t_flag;

#include "libft.h"
#include "libftprintf.h"
#include <stdarg.h>

int	ft_printf(const char *s, ...)
{
	va_list	a;
	int		i;
	int		len;
	char	*vrf;
	int	j;
	t_list	cont;

	va_start(a, s);
	i = 0;
	vrf = calloc(1, 1);
	len = 0;
	while (s[i])
	{
		if (s[i] == '%')
		{
			i++;
			while (ft_strchr("+-# .0123456789", s[i]))
			{
				i++;
				j++;
			}
			vrf = ft_substr(s, i - j, j);
			while (vrf[k])
			{
				num = 0;
				if (vrf[k] == '-')
				{
					k++;
					while (isnum(vrf[k]))
						num = picknum(num, vrf[k++]);
  					putminus(num);
  				}
				else if (vrf[k] == '+')
					putplus();
				else if (vrf[k++] == ' ' && !(strchr(vrf, '+')))
				{
					while (isnum(vrf[k]))
						num = picknum(num, vrf[k++]);
					putspace(num);
				}
				else if (vrf[k++] == '0' && !strchr(vrf, '-') && !strchr(vrf, '.'))
				{
					while (isnum(vrf[k]))
						num = picknum(num, vrf[k++]);
  					putzero(num);
  				}
				else if (vrf[k] == '#')
					puthash();
				if (isnum(vrf[k]))
    				{
					num = 0;
					while (isnum(vrf[k]))
						num = picknum(num, vrf[k++]);
					putwidth(num);
				}
				if (vrf[k++] == '.')
				{
					precision = 0;
					while (isnum(vrf[k]))
						precision = picknum(precision, vrf[k++]);
					putprecision(precision);
				}
				k++;
			}
		else
			len += (ft_putchar_fd(s[i], 1), 1);
		i++;
	}
	va_end(a);
	return (len);
}

int	picknum(int a, char b)
{
	int	numb;

	numb = a * 10 + (b - '\0');
	return (numb);
}

