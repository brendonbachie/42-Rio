#include <stdlib.h>
#include <stdio.h>

int main(void) {
    char *str;
    str = malloc(sizeof(char));
    str = "teste";

    printf("%s\n", str);
    printf("%p\n", &str);
    str = malloc(sizeof(char));
    printf("%s\n", str);
    printf("%p\n", &str);
    return (0);
}
