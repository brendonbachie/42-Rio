#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>

char	*get_next_line(int fd);
char	*gnl_strdup(char *str);
void	*gnl_calloc(size_t nmemb, size_t size);
size_t	gnl_strlen(const char *s);
char	*gnl_strchr(char *s, int c);
void	*gnl_memcpy(void *dest, const void *src, size_t n);
char *verif(char **str);

#endif
