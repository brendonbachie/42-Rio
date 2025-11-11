#include "get_next_line.h"

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

size_t	gnl_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	count;

	i = 0;
	j = 0;
	while (i < size && dst[i] != '\0')
		i++;
	count = i + gnl_strlen(src);
	if (i == size)
		return count;
	while (src[j] != '\0' && i + 1 < size)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (count);
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

char	*gnl_strjoin(char *s1, char *s2)
{
	char	*str;
	int	len;
	int	len_s1;
	
	if (!s1 || !s2)
		return (NULL);
	len_s1 = gnl_strlen(s1);
	len = len_s1 + (gnl_strlen(s2));
	str = gnl_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	gnl_memcpy(str, s1, len_s1);
	gnl_memcpy(str + len_s1, s2, gnl_strlen(s2));
	free (s1);
	return (str);
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
