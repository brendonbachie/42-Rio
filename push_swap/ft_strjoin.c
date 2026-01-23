/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:06:53 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/03 13:06:54 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		len;
	int		len_s1;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = strlen(s1);
	len = len_s1 + (strlen(s2));
	str = calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	memcpy(str, s1, len_s1);
	memcpy(str + len_s1, s2, strlen(s2));
	return (str);
}
