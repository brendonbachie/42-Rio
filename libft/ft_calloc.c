#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	unsigned char	*str;
	
	if (nmemb && size > SIZE_MAX / nmemb)
		return (NULL);
	total = nmemb * size;
	str = malloc(total);
	if (!str)
		return (NULL);
	ft_bzero(str, total);
	return (str);
}
