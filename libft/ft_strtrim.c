/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 12:49:36 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/10/25 12:49:37 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static	int	start(const char *s1, const char *set, int len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i <= len && set[j] != '\0')
	{
		if (s1[i] == set[j])
		{
			i++;
			j = 0;
		}
		else
			j++;
	}
	return (i);
}

static	int	end(const char *s1, const char *set, int n, int len)
{
	int	j;

	j = 0;
	while (set[j] != '\0' && len >= n)
	{
		if (s1[len] == set[j])
		{
			len--;
			j = 0;
		}
		else
			j++;
	}
	return (len);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	int			i;
	int			j;
	int			k;
	int			len;
	char		*str;

	if (!s1 || !set)
		return (NULL);
	j = ft_strlen(s1) - 1;
	i = start(s1, set, j);
	k = end(s1, set, i, j);
	len = k - i + 1;
	if (len < 0)
		len = 0;
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	if (len > 0)
		ft_memcpy(str, s1 + i, len);
	return (str);
}
