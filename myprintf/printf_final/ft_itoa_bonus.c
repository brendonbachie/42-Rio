/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:49:10 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/25 15:02:10 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	if (n == 0 || dest == src)
		return (dest);
	i = 0;
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

static char	*ft_strdup(const char *s)
{
	char	*str;
	int		len;

	len = ft_strlen(s) + 1;
	str = malloc(len);
	if (!str)
		return (NULL);
	ft_memcpy(str, s, len);
	return (str);
}

static int	numlen(long n)
{
	int		len;
	long	num;

	len = 0;
	num = n;
	if (num <= 0)
		len++;
	if (num < 0)
		num = -num;
	while (num > 0)
	{
		len++;
		num /= 10;
	}
	return (len);
}

static void	rstring(char *s)
{
	int		i;
	int		j;
	int		len;
	char	temp;

	i = 0;
	len = ft_strlen(s);
	j = len;
	while (i < len / 2)
	{
		temp = s[i];
		s[i] = s[j - 1];
		s[j - 1] = temp;
		i++;
		j--;
	}
}

char	*ft_itoa(long n)
{
	long	i;
	long	sign;
	char	*s;

	if (n == -2147483648)
		return (ft_strdup("2147483648"));
	sign = n;
	if (n < 0)
		sign = -sign;
	i = 0;
	s = malloc(numlen(n) + 1 * sizeof(char));
	if (!s)
		return (NULL);
	while (sign >= 10)
	{
		s[i++] = sign % 10 + '0';
		sign /= 10;
	}
	s[i++] = sign % 10 + '0';
	s[i] = '\0';
	rstring(s);
	return (s);
}
