#include "libftprintf.h"

int	ft_put_uns_fd(unsigned int n, int fd)
{
	int	count;
	
	count = 0;
	if (n >= 10)
		count += ft_put_uns_fd(n / 10, fd);
	write(fd, &"0123456789"[n % 10], 1);
	count += 1;
	return (count);
}
