#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "libftprintf.h"
#include "libft.h"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

// --- Macros para testes ---

// Sem argumentos variádicos
#define RUN0(desc, fmt) do { \
    char expected[500]; \
    int ret_ft, ret_std; \
    sprintf(expected, fmt); \
    ret_std = (int)strlen(expected); \
    ret_ft = ft_printf(fmt); \
    if (ret_ft == ret_std) \
        printf("[%02d] %s✅ OK%s | %s (len=%d)\n", test_i++, GREEN, RESET, desc, ret_ft); \
    else \
        printf("[%02d] %s❌ FAIL%s | %s (len=%d, esperado=%d)\n", test_i++, RED, RESET, desc, ret_ft, ret_std); \
} while(0)

// Com argumentos variádicos
#define RUN(desc, fmt, ...) do { \
    char expected[500]; \
    int ret_ft, ret_std; \
    sprintf(expected, fmt, __VA_ARGS__); \
    ret_std = (int)strlen(expected); \
    ret_ft = ft_printf(fmt, __VA_ARGS__); \
    if (ret_ft == ret_std) \
        printf("[%02d] %s✅ OK%s | %s (len=%d)\n", test_i++, GREEN, RESET, desc, ret_ft); \
    else \
        printf("[%02d] %s❌ FAIL%s | %s (len=%d, esperado=%d)\n", test_i++, RED, RESET, desc, ret_ft, ret_std); \
} while(0)

// -------------------------------------------------------------

int main(void)
{
    int test_i = 1;
    int a = 42;
    char *str = "teste";
    char *null_str = NULL;

    printf("\n===== TESTES AUTOMATIZADOS FT_PRINTF =====\n\n");

    // 1–10: básicos
    RUN0("Texto simples", "Texto simples\n");
    RUN("Char simples", "Char: %c\n", 'A');
    RUN("String", "String: %s\n", str);
    RUN("String NULL", "String NULL: %s\n", null_str);
    RUN("Decimal", "Int: %d\n", 123);
    RUN("Negativo", "Negativo: %i\n", -456);
    RUN("Zero", "Zero: %d\n", 0);
    RUN("Unsigned max", "Unsigned: %u\n", 4294967295U);
    RUN("Hex minúsculo", "Hexa min: %x\n", 0x1234abcd);
    RUN("Hex maiúsculo", "Hexa mai: %X\n", 0x1234ABCD);

    // 11–20: ponteiros e combinações
    RUN("Pointer variável", "Pointer: %p\n", &a);
    RUN("Pointer NULL", "Pointer NULL: %p\n", NULL);
    RUN0("Percent", "Percent: %%\n");
    RUN("Múltiplos chars", "%c%c%c\n", 'A', 'B', 'C');
    RUN("Múltiplas strings", "%s%s%s\n", "A", "B", "C");
    RUN("Múltiplos decimais", "%d%d%d\n", 1, 22, 333);
    RUN("Múltiplos unsigned", "%u%u%u\n", 1U, 22U, 333U);
    RUN("Múltiplos hex min", "%x %x %x\n", 0, 1, 15);
    RUN("Múltiplos hex mai", "%X %X %X\n", 0, 1, 15);
    RUN("Mix curto", "Mix: %d %s %c\n", 9, "abc", 'Z');

    // 21–30: combinações complexas
    RUN("Mix longo", "Mix longo: %d %s %p %u %x %%\n", -99, "test", &a, 1234U, 255);
    RUN("Hex negativos", "Hex negativos: %x %X\n", -1, -42);
    RUN("Grande número", "Grande: %d\n", 1000000);
    RUN("String longa", "String longa: %s\n", "abcdefghijklmnopqrstuvwxyz");
    RUN("String vazia", "String vazia: \"%s\"\n", "");
    RUN("INT_MIN/MAX", "INT_MIN/MAX: %d %d\n", INT_MIN, INT_MAX);
    RUN("UINT_MAX", "Unsigned MAX: %u\n", UINT_MAX);
    RUN("Hex 0", "Hex zero: %x %X\n", 0, 0);
    RUN("Mix completo", "Mix: %d %u %x %X %c %s %%\n", -42, 42U, 255, 255, 'Z', "ok");
    RUN("Char especial", "Chars: %c %c %c\n", '\n', '\t', ' ');

    // 31–40: sequências
    RUN("Seq números", "%d %d %d %d\n", 1, 2, 3, 4);
    RUN("Seq unsigned", "%u %u %u %u\n", 10U, 20U, 30U, 40U);
    RUN("Seq hex", "%x %x %x %x\n", 10, 11, 12, 13);
    RUN("Seq chars", "%c %c %c %c\n", 'a', 'b', 'c', 'd');
    RUN("Seq strings", "%s %s %s %s\n", "um", "dois", "tres", "quatro");
    RUN("Pointer repetido", "%p %p %p\n", &a, &a, &a);
    RUN("Mix variado", "%d %s %x %X %u %c %%\n", 123, "txt", 15, 15, 99U, '!');
    RUN("Zeros variados", "%d %u %x %X\n", 0, 0U, 0, 0);
    RUN("Negativos mistos", "%d %i %x %X\n", -1, -99, -1, -99);
    RUN("String repetida", "%s%s%s%s\n", "x", "x", "x", "x");

    // 41–70: testes automáticos repetidos
    for (int i = 41; i <= 70; i++) {
        RUN("Loop auto", "Teste %d: %d %u %x %X %c %s %%\n",
            i, i, (unsigned)i, i, i, 'A', "ok");
    }

    printf("\n===== FIM DOS TESTES =====\n");
    return 0;
}
