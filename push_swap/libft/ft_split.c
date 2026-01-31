/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:57:34 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/03 12:57:35 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	count_words(const char *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if ((s[i] != c && (i == 0 || s[i - 1] == c)))
			count++;
		i++;
	}
	return (count);
}

static	int	word_len(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static	void	word_cpy(char *dst, const char *src, char c)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != c)
	{
		dst[i] = src[i];
		i++;
	}
}

void	free_matrix(char **str)
{
	int	j;

	j = 0;
	while (str[j] != NULL)
		j++;
	while (j >= 0)
	{
		free(str[j]);
		j--;
	}
	free (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		j;
	int		k;
	int		len;

	k = 0;
	j = 0;
	str = ft_calloc(count_words(s, c) + 1, sizeof(char *));
	if (!s || !str)
		return (NULL);
	while (s[k])
	{
		while (s[k] == c)
			k++;
		if (s[k])
		{	
			len = word_len((char *)s + k, c);
			str[j] = ft_calloc(len + 1, sizeof(char));
			if (!str[j])
				return (free_matrix(str), NULL);
			word_cpy(str[j++], s + k, c);
			k += len;
		}
	}
	return (str);
}
