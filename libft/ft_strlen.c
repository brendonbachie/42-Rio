#include "libft.h"

size_t	ft_strlen(const char *s)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (s[i] != '\0')
    {
        count++;
        i++;
    }
    return (count);
}
