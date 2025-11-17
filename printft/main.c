#include "libftprintf.h"  // ou onde você tiver declarado ft_printf
#include <stdio.h>

int main(void)
{
    int a = 42;
    int b = -7;
    char c = 'Z';
    char *str = "Hello";

    ft_printf("Test 1: %%d -> %d\n", a);
    ft_printf("Test 2: %%+d -> %+d\n", a);
    ft_printf("Test 3: %%5d -> %5d\n", a);
    ft_printf("Test 4: %%05d -> %05d\n", a);
    ft_printf("Test 5: %%c -> %c\n", c);
    ft_printf("Test 6: %%s -> %s\n", str);
    ft_printf("Test 7: %%d %%d -> %d %d\n", a, b);

    return 0;
}

