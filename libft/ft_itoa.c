/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:49:10 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/03 12:49:11 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

static	int	numlen(int n)
{
	int	len;
	int	num;

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

static	void	rstring(char *s)
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

char	*ft_itoa(int n)
{
	int		i;
	int		sign;
	char	*s;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	sign = n;
	if (n < 0)
		sign = -sign;
	i = 0;
	s = ft_calloc(numlen(n) + 1, sizeof(char));
	if (!s)
		return (NULL);
	while (sign >= 10)
	{
		s[i++] = sign % 10 + '0';
		sign /= 10;
	}
	s[i++] = sign % 10 + '0';
	if (n < 0)
		s[i++] = '-';
	rstring(s);
	return (s);
}