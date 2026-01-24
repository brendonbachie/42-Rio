#ifndef ERRORS_H
# define ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

typedef struct s_elements
{
	int number;
	int index;
	struct s_elements *alvo;
	int custo_a;
	int custo_b;
	int custo_total;
	int index_b;
	struct s_elements *next;
} t_elements;

t_elements *ft_lists(char **argumentos);
void ft_lstadd_back(t_elements **lst, t_elements *new);
void ft_lstclear(t_elements **lst);
void    ate3(t_elements **stack_a);
int ft_errors(int quantidade, char **argumentos);
char	**ft_split(char const *s, char c);
t_elements	*ft_lstnew(int number);
int ft_isnum(char **str);
char    **ft_splitejoin(int quantidade, char **argumentos);
char	*ft_strjoin(char const *s1, char const *s2);
int	ft_lstsize(t_elements **lst);
void    sa(t_elements **stack_a);
void    sb(t_elements **stack_b);
void    ss(t_elements **stack_a, t_elements **stack_b);
void    pa(t_elements **stack_a, t_elements **stack_b);
void    pb(t_elements **stack_a, t_elements **stack_b);
void    ra(t_elements **stack_a);
void    rb(t_elements **stack_b);
void    rr(t_elements **stack_a, t_elements **stack_b);
void    rra(t_elements **stack_a);
void    rrb(t_elements **stack_b);
void    rrr(t_elements **stack_a, t_elements **stack_b);
void    three(t_elements **stack_a);
void    turco(t_elements **stack_a, t_elements **stack_b);
int ft_mod(int a);
void    index(t_elements *stack);
void    index_b(t_elements *stack_a, t_elements *stack_b);
void    alvo(t_elements *stack_a, t_elements *stack_b);
void    custo(t_elements *stack_a, t_elements *stack_b);
void    custo_total(t_elements *stack);
t_elements	*push_min_custo(t_elements *stack_a);
void    mov_final(t_elements **stack_a, t_elements **stack_b, t_elements *min_custo);
void    volta(t_elements **stack_a, t_elements **stack_b);
void    mov_final2(t_elements **stack_a, t_elements **stack_b, t_elements *min_custo);

#endif
