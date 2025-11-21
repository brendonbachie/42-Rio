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
	int	i;
	char	*r;
	 
	p = *str;
	i = -1;
	if (p[i + 1] == '\0')
	{
		*str = free_and_null(*str);
		return (NULL);
	}
	r = gnl_calloc(gnl_strlen(p) + 2, sizeof(char));
	if (!r) 
		return NULL;
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
	if (!s1)
		return (NULL);
	len_s1 = gnl_strlen(s1[0]);
	len = len_s1 + (gnl_strlen(s2));
	str = gnl_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	gnl_memcpy(str, *s1, len_s1);
	gnl_memcpy(str + len_s1, s2, gnl_strlen(s2));
	free (*s1);
	return (str);
}

char *get_next_line(int fd)
{
	static char *lidos[MAX_FD];
	char *ch;
	int	len;
	
	if (fd < 0 || fd >= MAX_FD)
		return (NULL);
	ch = gnl_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!ch)
		return (NULL);
	if (!lidos[fd])
		lidos[fd] = gnl_calloc(1, 1);
	len = read(fd, ch, BUFFER_SIZE);
	while (len > 0)
	{
		ch[len] = '\0';
		lidos[fd] = ft_strjoin(&lidos[fd], ch);
		if (gnl_strchr(lidos[fd], '\n') || len < BUFFER_SIZE)
		{
			free(ch);
			return (ret(&lidos[fd]));
		}
		len = read(fd, ch, BUFFER_SIZE);
	}
	free(ch);
	return (ret(&lidos[fd]));
}



/*
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
}*/
