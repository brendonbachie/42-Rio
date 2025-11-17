#include "libftprintf.h"

int	plus(va_list ap, char c, char b)
{
	unsigned int	u;
	int		d;
	int             count;

	d = va_arg(ap, int);
	count = 0;
	if (d >= 0 && c == '+')
		count += write(1, "+", 1);
	else if (d >= 0 && c == ' ')
		count += write(1, " ", 1);
	else if (c == '#')
	{
		if (b == 'x')
			count += write(1, "0x", 2);
		else if (b == 'X')
			count += write(1, "0X", 2);	
		count += (ft_hex_fd((unsigned int)d, 1, b));
	}
	if (b == 'd' || b == 'i')
	{
		ft_putnbr_fd(d, 1);
		count += (ft_numlen(d));
	}
	else if (b == 'u')
	{
		u = va_arg(ap, unsigned int);
		count += (ft_put_uns_fd(u, 1));
	}
	return (count);
}
