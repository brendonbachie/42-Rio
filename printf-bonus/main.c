#include <stdio.h>
#include <limits.h>
#include "ft_printf_bonus.h"

int main(void)
{
    int     i = -42;
    int     pos = 42;
    unsigned int u = 4294967295;
    int     hex = 48879;
    char    c = 'A';
    char    *str = "Hello 42";
    char    *null_str = NULL;
    void    *ptr = &i;

    printf("=========== BASIC ===========\n");
    printf("ORIG: %d\n", i);
    ft_printf("FT  : %d\n\n", i);

    printf("ORIG: %i\n", pos);
    ft_printf("FT  : %i\n\n", pos);

    printf("ORIG: %u\n", u);
    ft_printf("FT  : %u\n\n", u);

    printf("ORIG: %x\n", hex);
    ft_printf("FT  : %x\n\n", hex);

    printf("ORIG: %X\n", hex);
    ft_printf("FT  : %X\n\n", hex);

    printf("ORIG: %p\n", ptr);
    ft_printf("FT  : %p\n\n", ptr);

    printf("ORIG: %c\n", c);
    ft_printf("FT  : %c\n\n", c);

    printf("ORIG: %s\n", str);
    ft_printf("FT  : %s\n\n", str);

    printf("ORIG: %s\n", null_str);
    ft_printf("FT  : %s\n\n", null_str);

    printf("=========== WIDTH ===========\n");
    printf("ORIG: |%10d|\n", i);
    ft_printf("FT  : |%10d|\n\n", i);

    printf("ORIG: |%-10d|\n", i);
    ft_printf("FT  : |%-10d|\n\n", i);

    printf("=========== ZERO FLAG ===========\n");
    printf("ORIG: |%010d|\n", i);
    ft_printf("FT  : |%010d|\n\n", i);

    printf("=========== PRECISION ===========\n");
    printf("ORIG: |%.5d|\n", i);
    ft_printf("FT  : |%.5d|\n\n", i);

    printf("ORIG: |%.3s|\n", str);
    ft_printf("FT  : |%.3s|\n\n", str);

    printf("=========== WIDTH + PRECISION ===========\n");
    printf("ORIG: |%10.5d|\n", i);
    ft_printf("FT  : |%10.5d|\n\n", i);

    printf("ORIG: |%-10.5d|\n", i);
    ft_printf("FT  : |%-10.5d|\n\n", i);

    printf("=========== PLUS & SPACE ===========\n");
    printf("ORIG: |%+d|\n", pos);
    ft_printf("FT  : |%+d|\n\n", pos);

    printf("ORIG: |% d|\n", pos);
    ft_printf("FT  : |% d|\n\n", pos);

    printf("=========== HASH FLAG ===========\n");
    printf("ORIG: |%#x|\n", hex);
    ft_printf("FT  : |%#x|\n\n", hex);

    printf("ORIG: |%#X|\n", hex);
    ft_printf("FT  : |%#X|\n\n", hex);

    printf("=========== MIXED FLAGS CHAOS ===========\n");
    printf("ORIG: |%#08x|\n", hex);
    ft_printf("FT  : |%#08x|\n\n", hex);

    printf("ORIG: |%-#10.5x|\n", hex);
    ft_printf("FT  : |%-#10.5x|\n\n", hex);

    printf("ORIG: |%+010d|\n", pos);
    ft_printf("FT  : |%+010d|\n\n", pos);

    printf("ORIG: |%-+10.5d|\n", pos);
    ft_printf("FT  : |%-+10.5d|\n\n", pos);

    printf("=========== EDGE CASES ===========\n");
    printf("ORIG: |%.0d|\n", 0);
    ft_printf("FT  : |%.0d|\n\n", 0);

    printf("ORIG: |%5.0d|\n", 0);
    ft_printf("FT  : |%5.0d|\n\n", 0);

    printf("ORIG: |%.0x|\n", 0);
    ft_printf("FT  : |%.0x|\n\n", 0);

    printf("ORIG: |%#x|\n", 0);
    ft_printf("FT  : |%#x|\n\n", 0);

    printf("=========== INT LIMITS ===========\n");
    printf("ORIG: %d\n", INT_MAX);
    ft_printf("FT  : %d\n\n", INT_MAX);

    printf("ORIG: %d\n", INT_MIN);
    ft_printf("FT  : %d\n\n", INT_MIN);
    printf("\n=========== FLAG CONFLICTS ===========\n");


printf("\n=========== ZERO & PRECISION EDGE ===========\n");

printf("ORIG: |%.0d|\n", 0);
ft_printf("FT  : |%.0d|\n\n", 0);

printf("ORIG: |%08.0d|\n", 0);
ft_printf("FT  : |%08.0d|\n\n", 0); // deve imprimir só espaços

printf("\n=========== HASH WEIRD CASES ===========\n");

printf("ORIG: |%#x|\n", 0);
ft_printf("FT  : |%#x|\n\n", 0); // não deve imprimir 0x

printf("ORIG: |%#.0x|\n", 0);
ft_printf("FT  : |%#.0x|\n\n", 0); // nada

printf("ORIG: |%#8x|\n", 0);
ft_printf("FT  : |%#8x|\n\n", 0); // só padding

printf("\n=========== SIGN + WIDTH + PRECISION CHAOS ===========\n");

printf("ORIG: |%+10.5d|\n", -42);
ft_printf("FT  : |%+10.5d|\n\n", -42);

printf("ORIG: |% 10.5d|\n", 42);
ft_printf("FT  : |% 10.5d|\n\n", 42);

printf("\n=========== POINTER EDGE ===========\n");

printf("ORIG: |%20p|\n", NULL);
ft_printf("FT  : |%20p|\n\n", NULL);

printf("ORIG: |%-20p|\n", NULL);
ft_printf("FT  : |%-20p|\n\n", NULL);

printf("\n=========== CHAR EDGE ===========\n");

printf("ORIG: |%5c|\n", 0);
ft_printf("FT  : |%5c|\n\n", 0); // caractere nulo

printf("ORIG: |%-5c|\n", 0);
ft_printf("FT  : |%-5c|\n\n", 0);

printf("\n=========== STRING EDGE ===========\n");

printf("ORIG: |%10.3s|\n", "abcdef");
ft_printf("FT  : |%10.3s|\n\n", "abcdef");

printf("ORIG: |%-10.3s|\n", "abcdef");
ft_printf("FT  : |%-10.3s|\n\n", "abcdef");

printf("\n=========== RETURN TEST ==========\n");

int r1 = printf("|%5c|\n", 0);
int r2 = ft_printf("|%5c|\n", 0);
printf("ORIG RET: %d\n", r1);
printf("FT   RET: %d\n\n", r2);

r1 = printf("|%.0d|\n", 0);
r2 = ft_printf("|%.0d|\n", 0);
printf("ORIG RET: %d\n", r1);
printf("FT   RET: %d\n\n", r2);

r1 = printf("|%#08x|\n", 42);
r2 = ft_printf("|%#08x|\n", 42);
printf("ORIG RET: %d\n", r1);
printf("FT   RET: %d\n\n", r2);

    return (0);
}

