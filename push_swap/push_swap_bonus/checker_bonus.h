/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 18:59:57 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/02 19:00:02 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_BONUS_H
# define CHECKER_BONUS_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <limits.h>
# include "../libft/libft.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

typedef struct s_node
{
	struct s_node	*target;
	struct s_node	*next;
	int				number;
	int				index_node;
	int				cost_a;
	int				cost_b;
	int				total_cost;
	int				target_index;
	int				index;
}	t_node;

int			is_ordened(t_node *stack_a);
int			ft_errors(char **args, t_node *stack);
int			ft_mod(int a);
int			ft_isnum(char **str);
int			ft_lstsize(t_node **lst);
int			has_duplicate(t_node *stack);
int			instructions(t_node **stack_a, t_node **stack_b, char *str);
int			args_vrf(char **argv, int argc);
void		ft_lstadd_back(t_node **lst, t_node *new);
void		ft_lstclear(t_node **lst);
void		sa(t_node **stack_a);
void		sb(t_node **stack_b);
void		ss(t_node **stack_a, t_node **stack_b);
void		pa(t_node **stack_a, t_node **stack_b);
void		pb(t_node **stack_a, t_node **stack_b);
void		ra(t_node **stack_a);
void		rb(t_node **stack_b);
void		rr(t_node **stack_a, t_node **stack_b);
void		rra(t_node **stack_a);
void		return_error(t_node **stack, char **args);
void		rrb(t_node **stack_b);
void		rrr(t_node **stack_a, t_node **stack_b);
void		free_matrix(char **str);
void		*free_and_null(char *ptr);
void		checker(t_node **stack_a, t_node **stack_b);
char		*get_next_line(int fd);
char		*ret(char **str);
char		**ft_splitandjoin(char **args);
t_node		*ft_lstnew(int number);
t_node		*ft_lists(char **argumentos);

#endif
