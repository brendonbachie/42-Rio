#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	
	if (start >= ft_strlen(s))
	{
		str = ft_calloc(1, 1);
		return (str);
	}
	if ((len + start) > ft_strlen(s))
		len = ft_strlen(s) - start;
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	ft_memmove(str, s + start, len);
	return (str);
}
