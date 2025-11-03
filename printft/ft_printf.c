#include "libft.h"
#include "libftprintf.h"
#include <stdarg.h>

int	ft_printf(const char *str , ...)
{
	va_list	ap;
	int	i;
	int	len;

	i = 0;
	len = 0;
	va_start(ap, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%')
		{
			i++;
			if (ft_strchr("cpxX", str[i]))
				len += ft_convert_char(ap, str[i]);
			else if (ft_strchr("diu%", str[i]))
				len += ft_convert_num(ap, str[i]);
			else if (str[i] == 's')
				len += ft_convert_string(ap);
		}
		else
			len += (ft_putchar_fd(str[i], 1), 1);
		i++;
	}
	va_end (ap);
	return(len);
}
/*
#include "libft.h"
#include <stdio.h>

int	main(void)
{
	char	*x = "34";
	int	t = 'n';
	int	a = -35258564;
	void	*p = "35";
	
	printf("%d\n", ft_printf("\n%s\n%x\n%d\n%p\n------", x, t, a, p));
	printf("%d\n", printf("\n%s\n%x\n%d\n%p\n------", x, t, a, p));
	printf("%d\n", printf("%u\n", a));
	printf("%d\n", ft_printf("%u\n", a));
	return (0);
}*/





















