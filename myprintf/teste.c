#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "libftprintf.h"
#include "libft.h"

int	main(void)
{
	int	a = 10;
	char *s = "teste";
	
	printf("%d%s%y", a, s);
	return (0);
}
