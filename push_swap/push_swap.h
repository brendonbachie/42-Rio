/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:09:41 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 19:58:10 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>

typedef struct s_elements
{
	struct s_elements	*alvo;
	struct s_elements	*next;
	int					number;
	int					index;
	int					custo_a;
	int					custo_b;
	int					custo_total;
	int					index_b;
}	t_elements;

int			ft_errors(int quantidade, char **argumentos);
int			ft_mod(int a);
int			ft_isnum(char **str);
int			ft_lstsize(t_elements **lst);
void		ft_lstadd_back(t_elements **lst, t_elements *new);
void		ft_lstclear(t_elements **lst);
void		mount_node_go(t_elements **stack_a, t_elements **stack_b);
void		ate3(t_elements **stack_a);
void		mount_node_back(t_elements **stack_a, t_elements **stack_b);
void		sa(t_elements **stack_a);
void		sb(t_elements **stack_b);
void		ss(t_elements **stack_a, t_elements **stack_b);
void		pa(t_elements **stack_a, t_elements **stack_b);
void		pb(t_elements **stack_a, t_elements **stack_b);
void		ra(t_elements **stack_a);
void		rb(t_elements **stack_b);
void		rr(t_elements **stack_a, t_elements **stack_b);
void		rra(t_elements **stack_a);
void		rrb(t_elements **stack_b);
void		rrr(t_elements **stack_a, t_elements **stack_b);
void		three(t_elements **stack_a);
void		turco(t_elements **stack_a, t_elements **stack_b);
void		indice(t_elements *stack);
void		index_b(t_elements *stack_a, t_elements *stack_b);
void		last_move(t_elements **stack_a);
void		alvo(t_elements *stack_a, t_elements *stack_b);
void		custo(t_elements *stack_a, t_elements *stack_b);
void		custo_total(t_elements *stack);
void		mov_final(t_elements **stack_a,
				t_elements **stack_b, t_elements *min_custo);
void		volta(t_elements *stack_a, t_elements *stack_b);
void		mov_final2(t_elements **stack_b,
				t_elements **stack_a, t_elements *min_custo);
void		free_matriz(char **str);
void		soma_custos(t_elements *stack_a);
void		mov_a(t_elements **stack, int custo);
void		mov_b(t_elements **stack, int custo);
char		*ft_strjoin(char **s1, char const *s2);
char		**ft_splitejoin(int quantidade, char **argumentos);
char		**ft_split(char const *s, char c);
t_elements	*ft_lstnew(int number);
t_elements	*ft_lists(char **argumentos);
t_elements	*menor_valor(t_elements *stack_a);
t_elements	*maior_valor(t_elements *stack);
t_elements	*push_min_custo(t_elements *stack_a);

#endif
