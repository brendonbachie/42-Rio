/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:09:41 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/26 06:27:10 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <limits.h>
# include "libft/libft.h"

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
int			args_vrf(char **argv, int argc);
int			ft_errors(char **args, t_node *stack);
int			ft_mod(int a);
int			ft_isnum(char **str);
int			ft_lstsize(t_node **lst);
int			has_duplicate(t_node *stack);
void		five(t_node **stack_a, t_node **stack_b);
void		final_index(t_node *stack_a);
void		ft_lstadd_back(t_node **lst, t_node *new);
void		ft_lstclear(t_node **lst);
void		small_numbers(t_node **stack_a);
void		b_to_a(t_node **stack_a, t_node **stack_b);
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
void		three(t_node **stack_a);
void		algorithm(t_node **stack_a, t_node **stack_b);
void		index_node(t_node *stack);
void		index_target(t_node *stack_a, t_node *stack_b);
void		last_move(t_node **stack_a);
void		target(t_node *stack_a, t_node *stack_b);
void		cost(t_node *stack_a, t_node *stack_b);
void		total_cost(t_node *stack);
void		push_swap(t_node **head, t_node **head2);
void		chose_move(t_node **stack_b,
				t_node **stack_a, t_node *lowest_cost);
void		send_to_b(t_node **stack_a, t_node **stack_b);
void		mov_a(t_node **stack, int cost);
void		mov_b(t_node **stack, int cost);
void		free_matrix(char **str);
char		**ft_splitandjoin(char **args);
char		**ft_split(char const *s, char c);
t_node		*ft_lstnew(int number);
t_node		*ft_lists(char **arguments);
t_node		*lowest_value(t_node *stack_a);
t_node		*greater_value(t_node *stack);
t_node		*lowest_cost(t_node *stack_b);
#endif
