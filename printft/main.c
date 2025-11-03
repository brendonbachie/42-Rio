#include <stdio.h>
#include <limits.h>
#include "libftprintf.h"

int main(void)
{
	/*int len1, len2;
	char c = 'A';
	char *str = "Hello, ft_printf!";
	int d = -12345;
	unsigned int u = 4294967295;
	void *p = &d;
	int zero = 0;

	printf("===== Testando %%c =====\n");
	len1 = printf("printf:    [%c]\n", c);
	len2 = ft_printf("ft_printf: [%c]\n", c);
	printf("len: printf=%d, ft_printf=%d\n\n", len1, len2);

	printf("===== Testando %%s =====\n");
	len1 = printf("printf:    [%s]\n", str);
	len2 = ft_printf("ft_printf: [%s]\n", str);
	printf("len: printf=%d, ft_printf=%d\n\n", len1, len2);

	printf("===== Testando %%p =====\n");
	len1 = printf("printf:    [%p]\n", p);
	len2 = ft_printf("ft_printf: [%p]\n", p);
	printf("len: printf=%d, ft_printf=%d\n\n", len1, len2);

	printf("===== Testando %%d/%%i =====\n");
	len1 = printf("printf:    [%d] [%i]\n", d, d);
	len2 = ft_printf("ft_printf: [%d] [%i]\n", d, d);
	printf("len: printf=%d, ft_printf=%d\n\n", len1, len2);

	printf("===== Testando %%u =====\n");
	len1 = printf("printf:    [%u]\n", u);
	len2 = ft_printf("ft_printf: [%u]\n", u);
	printf("len: printf=%d, ft_printf=%d\n\n", len1, len2);

	printf("===== Testando %%x/%%X =====\n");
	len1 = printf("printf:    [%x] [%X]\n", u, u);
	len2 = ft_printf("ft_printf: [%x] [%X]\n", u, u);
	printf("len: printf=%d, ft_printf=%d\n\n", len1, len2);

	printf("===== Testando %% com zero e %%c %%s =====\n");
	len1 = printf("printf:    %% %c %s %d\n", c, str, zero);
	len2 = ft_printf("ft_printf: %% %c %s %d\n", c, str, zero);
	printf("len: printf=%d, ft_printf=%d\n\n", len1, len2);

	printf("===== Testando string NULL =====\n");
	char *null_str = NULL;
	len1 = printf("printf:    [%s]\n", null_str);
	len2 = ft_printf("ft_printf: [%s]\n", null_str);
	printf("len: printf=%d, ft_printf=%d\n\n", len1, len2);
	
	*/printf("===== Testando ZERO =====\n");
	//printf(0);
	ft_printf(0);

	return 0;
}

