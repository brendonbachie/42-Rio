#ifndef ERRORS_H
# define ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct s_elements
{
	int number;
	struct s_elements *next;
} t_elements;

int ft_errors(int quantidade, char **argumentos);
char	**ft_split(char const *s, char c);
t_elements	*ft_lstnew(int number);
int ft_isnum(char **str);
char    **ft_splitejoin(int quantidade, char **argumentos);
char	*ft_strjoin(char const *s1, char const *s2);

#endif
