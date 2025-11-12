#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 42
#define MAX_FD 1024

char	*search_line(char *str, int fd);

char	*memory(char *str, char *str2, int fd);

char	*get_next_line(int fd)
{
	char	*retorno;
	static char	*lidos[MAX_FD];
	char	*ch;
	int	len;
	
	ch = calloc(BUFFER_SIZE + 1, sizeof(char));
	retorno = malloc(BUFFER_SIZE + 1 * sizeof(char));
	if (!ch || !retorno || fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!lidos[fd])
		lidos[fd] = calloc(1, 1);
	if (lidos[fd] && *lidos[fd])
	{
		retorno = search_line(lidos[fd], fd);
		lidos[fd] = memory_and_free(lidos[fd], ch, fd);
		return (retorno);
	}
	while ((len = read(fd, ch, BUFFER_SIZE)) > 0)
	{
		lidos[fd] = realloc(lidos[fd], len + strlen(lidos[fd]) + 1);
		retorno = realloc(retorno, len + strlen(lidos[fd]) + 1);
		strlcat(lidos[fd], ch, len);
		retorno = search_line(lidos[fd], fd);
		lidos[fd] = memory_and_free(lidos[fd], ch, fd);
	}
	return (retorno);
}

char	*search_line(char *str, int fd)
{
	char	*ret;
	int	i;
	
	i = 0;
	ret = malloc((strlen(str) + 1) * sizeof(char));
	while (str[i] != '\n' && str[i] != '\0')
	{
		ret[i] = str[i];
		i++;
	}
	if (str[i] == '\n' || str[i] == '\0')
	{
		if (str[i] == '\n')
		{
			ret[i] = '\n';
			ret[i + 1] = '\0';
		}
		else
			ret[i] = '\0';
	}
	return (ret);
}

char	*memory_and_free(char *str, char *str2, int fd)
{
	int	i;
	char	*new;
	
	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\n')
	{
		new = strdup(str + i + 1);
		free (str);
		free (str2);
		return (new);
	}
	else
	{
		free (str2);
		free(str);
		return (NULL);
	}
}

