#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int	len;
	
	len = ft_strlen(s) + 1;
	str = malloc(len);
	if (!str)
		return (NULL);
	ft_memcpy(str, s, len);
	return (str);
}
