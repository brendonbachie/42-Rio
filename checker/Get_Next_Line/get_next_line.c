/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 11:14:22 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/12/12 11:14:25 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*free_and_null(char *ptr)
{
	free(ptr);
	ptr = NULL;
	return (ptr);
}

char	*ret(char **str)
{
	char	*p;
	int		i;
	char	*r;

	p = *str;
	i = -1;
	if (p[0] == '\0')
	{
		*str = free_and_null(*str);
		return (NULL);
	}
	r = gnl_calloc(gnl_strlen(p) + 2, sizeof(char));
	if (!r)
		return (NULL);
	while (p[++i] && p[i] != '\n')
		r[i] = p[i];
	if (p[i] == '\n')
		r[i++] = '\n';
	r[i] = '\0';
	if (p[i] == '\0')
		*str = free_and_null(*str);
	else
		*str = gnl_strdup(*str, p + i);
	return (r);
}

char	*ft_strjoin(char **s1, char *s2)
{
	char	*str;
	int		len;
	int		len_s1;

	if (!s2)
		return (NULL);
	if (!*s1)
		*s1 = gnl_calloc(1, 1);
	if (!s1 || !*s1)
		return (NULL);
	len_s1 = gnl_strlen(*s1);
	len = len_s1 + (gnl_strlen(s2));
	str = gnl_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	gnl_memcpy(str, *s1, len_s1);
	gnl_memcpy(str + len_s1, s2, gnl_strlen(s2));
	free (*s1);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*readed;
	char		*buffer;
	int			len;

	if (fd < 0)
		return (NULL);
	buffer = gnl_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	if (!readed)
		readed = gnl_calloc(1, 1);
	len = read(fd, buffer, BUFFER_SIZE);
	while (len > 0)
	{
		buffer[len] = '\0';
		readed = ft_strjoin(&readed, buffer);
		if (gnl_strchr(readed, '\n') || len < BUFFER_SIZE)
		{
			free(buffer);
			return (ret(&readed));
		}
		len = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (ret(&readed));
}
