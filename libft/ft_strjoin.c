#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int	len;
	int	len_s1;
	
	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len = len_s1 + (ft_strlen(s2));
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	memcpy(str, s1, len_s1);
	memcpy(str + len_s1, s2, ft_strlen(s2));
	return (str);
}

