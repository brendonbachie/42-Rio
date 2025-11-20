#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get_next_line_bonus.h"
#define MAX_FD 1024

char	*ret(char **str)
{
	char	*p;
	char	*r;
	int	i;
	char	*tmp;
	
	p = *str;
	i = 0;
	r = gnl_calloc(gnl_strlen(p) + 2, sizeof(char));
	while (p[i] != '\n')
	{
		r[i] = p[i];
		i++;
	}
	if (p[i] == '\n')
		r[i++] = '\n';
	r[i] = '\0';
	tmp = gnl_strdup(p + i);
	free (*str);
	*str = tmp;
	return (r);
}

char *verif(char **str)
{
	char *retorno;

	if (!*str || **str == '\0')
	{
		free(*str);
		*str = NULL;
		return (NULL);
	}
	if (strchr(*str, '\n'))
		return (ret(str));
	retorno = strdup(*str);
	free(*str);
	*str = NULL;
	return (retorno);
}



char	*ft_strjoin(char **s1, char *s2)
{
	char	*str;
	int		len;
	int		len_s1;

	if (!s2)
		return (NULL);
	if (!s1)
		return (NULL);
	len_s1 = gnl_strlen(*s1);
	len = len_s1 + (gnl_strlen(s2));
	str = gnl_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	gnl_memcpy(str, s1, len_s1);
	gnl_memcpy(str + len_s1, s2, gnl_strlen(s2));
	free (*s1);
	return (str);
}

char *get_next_line(int fd)
{
	static char *lidos[MAX_FD];
	char *ch;
	int len;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	ch = calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!ch)
		return (NULL);
	if (!lidos[fd])
		lidos[fd] = gnl_calloc(1, 1);
	while (read(fd, ch, BUFFER_SIZE))
	{
		lidos[fd] = ft_strjoin(&lidos[fd], ch);
		if (gnl_strchr(lidos[fd], '\n'))
		{
			free(ch);
			return (ret(&lidos[fd]));
		}
	}
	free(ch);
	return (verif(&lidos[fd]));
}


