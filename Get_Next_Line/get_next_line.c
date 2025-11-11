#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 2
#define MAX_FD 1024

char	*ret(char **str);

char	*ft_strjoin(char *s1, char *s2);

char	*verif(char **str);

char *get_next_line(int fd)
{
	static char *lidos[MAX_FD];
	char *retorno;
	char *ch;
	int len;

	if (fd < 0 || fd >= MAX_FD)
		return (NULL);
	ch = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!ch)
		return (NULL);
	if (!lidos[fd])
		lidos[fd] = calloc(1, 1);
	while ((len = read(fd, ch, BUFFER_SIZE)) > 0)
	{
		if (len > 0)
			ch[len] = '\0';
		lidos[fd] = ft_strjoin(lidos[fd], ch);
		if (strchr(lidos[fd], '\n'))
		{
			free(ch);
			return (ret(&lidos[fd]));
		}
	}
	free(ch);
	return (verif(&lidos[fd]));
}

char	*ret(char **str)
{
	char	*p;
	char	*r;
	int	i;
	char	*tmp;
	
	p = *str;
	i = 0;
	r = calloc(strlen(p) + 2, sizeof(char));
	while (p[i] && p[i] != '\n')
	{
		r[i] = p[i];
		i++;
	}
	if (p[i] == '\0') 
		r[i] = '\0';
	else 
	{
		r[i] = '\n';
		r[i + 1] = '\0';
		i++;
	}
	tmp = strdup(p + i);
	free (*str);
	*str = tmp;
	return (r);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		len;
	int		len_s1;

	if (!s2)
		return (NULL);
	if (!s1)
		s1 = calloc(1, 1);
	if (!s1)
		return (NULL);
	len_s1 = strlen(s1);
	len = len_s1 + (strlen(s2));
	str = calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	memcpy(str, s1, len_s1);
	memcpy(str + len_s1, s2, strlen(s2));
	free (s1);
	return (str);
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
