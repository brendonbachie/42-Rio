/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:05:24 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/01 18:44:33 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char *str, char *str2)
{
	char	*s;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen(str2);
	s = malloc(len + 1);
	if (!s)
		return (NULL);
	while (i < len)
	{
		s[i] = str2[i];
		i++;
	}
	s[i] = '\0';
	free(str);
	return (s);
}
