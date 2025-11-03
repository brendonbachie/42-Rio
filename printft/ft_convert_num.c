#include "libft.h"
#include "libftprintf.h"

int	ft_convert_num(va_list ap, char cvt)
{
	unsigned int	u;
	int		d;
	
	if (cvt == 'd' || cvt == 'i')
	{
		d = va_arg(ap, int);
		ft_putnbr_fd(d, 1);
		return (ft_numlen(d));
	}
	else if (cvt == 'u')
	{
		u = va_arg(ap, unsigned int);
		return (ft_put_uns_fd(u, 1));
	}
	else if (cvt == '%')
	{
		write(1, "%", 1);
		return (1);
	}
	return (0);
}

