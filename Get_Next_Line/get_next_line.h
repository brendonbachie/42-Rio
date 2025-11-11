#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

char	*get_next_line(int fd);
char	*gnl_strdup(char *str);
void	*gnl_calloc(size_t nmemb, size_t size);
size_t	gnl_strlen(const char *s);
size_t	gnl_strlcat(char *dst, const char *src, size_t size);
void	*gnl_memcpy(void *dest, const void *src, size_t n);
char	*gnl_strjoin(char *s1, char *s2);

#endif
