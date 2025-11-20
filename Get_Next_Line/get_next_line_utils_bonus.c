#include "get_next_line_bonus.h"

char	*gnl_strdup(char *str)
{
	char	*s;
	int	len;
	int	i;
	
	i = 0;
	len = gnl_strlen(str);
	s = malloc(len + 1);
	if (!s)
		return (NULL);
	while (i < len)
	{
		s[i] = str[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

void	*gnl_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	unsigned char	*str;
	size_t	i;
	
	i = 0;
	if (nmemb && size > SIZE_MAX / nmemb)
		return (NULL);
	total = nmemb * size;
	str = malloc(total);
	if (!str)
		return (NULL);
	while (i < total)
    	{
    		str[i] = '\0';
    		i++;
    	}
	return (str);
}

char	*gnl_strchr(char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

size_t	gnl_strlen(const char *s)
{
	size_t i;
	size_t count;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		count++;
		i++;
	}
	return (count);
}


void	*gnl_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t		i;

	if (n == 0 || dest == src)
		return (dest);
	i = 0;
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
