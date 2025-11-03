#include "libft.h"
#include "libftprintf.h"

int	ft_convert_string(va_list ap)
{
	size_t	len;
	char	*s;
	
	s = va_arg(ap, char *);
	if (!s)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	ft_putstr_fd(s, 1);
	len = ft_strlen(s);
	return (len);
}
