/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 11:14:22 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/02 19:00:26 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

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
	r = ft_calloc(ft_strlen(p) + 2, sizeof(char));
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
		*str = ft_strdup(*str, p + i);
	return (r);
}

char	*get_next_line(int fd)
{
	static char	*readed;
	char		*buffer;
	int			len;

	if (fd < 0)
		return (NULL);
	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	if (!readed)
		readed = ft_calloc(1, 1);
	len = read(fd, buffer, BUFFER_SIZE);
	while (len > 0)
	{
		buffer[len] = '\0';
		readed = ft_strjoin(&readed, buffer);
		if (ft_strchr(readed, '\n') || len < BUFFER_SIZE)
		{
			free(buffer);
			return (ret(&readed));
		}
		len = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (ret(&readed));
}
