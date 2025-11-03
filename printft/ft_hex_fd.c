#include "libft.h"

int	ft_hex_fd(unsigned long n, int fd, char caps)
{
	int	count;
	
	count = 0;
	if (n >= 16)
		count += ft_hex_fd(n / 16, fd, caps);
	if (caps == 'X')
		write(fd, &"0123456789ABCDEF"[n % 16], 1);
	else
		write(fd, &"0123456789abcdef"[n % 16], 1);
	count += 1;
	return (count);
}
