#include <stdio.h>
#include "ft_printf.h"

int main(void)
{
    int neg = -1;
    int zero = 0;
    int *ptr = NULL;
    char *str = NULL;

    printf("\n=========== INT EDGE CASES ===========\n\n");

    printf("printf     : |%d|\n", neg);
    printf("ft_printf  : |%d|\n\n", neg);

    printf("printf     : |%+d|\n", neg);
    printf("ft_printf  : |%+d|\n\n", neg);

    printf("printf     : |% d|\n", neg);
    printf("ft_printf  : |% d|\n\n", neg);

    printf("printf     : |%.0d|\n", zero);
    printf("ft_printf  : |%.0d|\n\n", zero);

    printf("printf     : |%5.0d|\n", zero);
    printf("ft_printf  : |%5.0d|\n\n", zero);

    printf("\n=========== POINTER / NIL CASES ===========\n\n");

    printf("printf     : |%p|\n", ptr);
    printf("ft_printf  : |%p|\n\n", ptr);

    printf("printf     : |%20p|\n", ptr);
    printf("ft_printf  : |%20p|\n\n", ptr);

    printf("printf     : |%-20p|\n", ptr);
    printf("ft_printf  : |%-20p|\n\n", ptr);

    printf("\n=========== STRING NULL CASES ===========\n\n");

    printf("printf     : |%s|\n", str);
    printf("ft_printf  : |%s|\n\n", str);

    printf("printf     : |%.3s|\n", str);
    printf("ft_printf  : |%.3s|\n\n", str);

    printf("printf     : |%10s|\n", str);
    printf("ft_printf  : |%10s|\n\n", str);

    printf("printf     : |%-10s|\n", str);
    printf("ft_printf  : |%-10s|\n\n", str);

    return 0;
}
