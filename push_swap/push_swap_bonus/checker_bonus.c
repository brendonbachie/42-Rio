/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 18:41:46 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/02 18:59:55 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

int	main(int argc, char **argv)
{
	t_node	*head;
	t_node	*head2;
	char	**splitandjoin;

	head = NULL;
	head2 = NULL;
	if (argc < 2)
		return (-1);
	if (args_vrf(argv, argc))
		return (0);
	splitandjoin = ft_splitandjoin(argv);
	head = ft_lists(splitandjoin);
	if (!head || ft_errors(splitandjoin, head))
	{
		return_error(&head, splitandjoin);
		return (0);
	}
	checker(&head, &head2);
	free_matrix(splitandjoin);
	return (0);
}

void	checker(t_node **stack_a, t_node **stack_b)
{
	char	*str;

	str = get_next_line(0);
	while (str != NULL)
	{
		if (!instructions(stack_a, stack_b, str))
		{
			free(str);
			write(2, "Error\n", 6);
			ft_lstclear(stack_a);
			ft_lstclear(stack_b);
			exit(1);
		}
		free(str);
		str = get_next_line(0);
	}
	if (is_ordened(*stack_a) && *stack_b == NULL)
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	ft_lstclear(stack_a);
	ft_lstclear(stack_b);
}

int	instructions(t_node **a, t_node **b, char *str)
{
	if (ft_strncmp(str, "sa\n", 3) == 0)
		sa(a);
	else if (ft_strncmp(str, "sb\n", 3) == 0)
		sb(b);
	else if (ft_strncmp(str, "ss\n", 3) == 0)
		ss(a, b);
	else if (ft_strncmp(str, "pa\n", 3) == 0)
		pa(a, b);
	else if (ft_strncmp(str, "pb\n", 3) == 0)
		pb(a, b);
	else if (ft_strncmp(str, "ra\n", 3) == 0)
		ra(a);
	else if (ft_strncmp(str, "rb\n", 3) == 0)
		rb(b);
	else if (ft_strncmp(str, "rr\n", 3) == 0)
		rr(a, b);
	else if (ft_strncmp(str, "rra\n", 4) == 0)
		rra(a);
	else if (ft_strncmp(str, "rrb\n", 4) == 0)
		rrb(b);
	else if (ft_strncmp(str, "rrr\n", 4) == 0)
		rrr(a, b);
	else
		return (0);
	return (1);
}

void	free_matrix(char **str)
{
	int	j;

	j = 0;
	while (str[j] != NULL)
		j++;
	while (j >= 0)
	{
		free(str[j]);
		j--;
	}
	free(str);
}

int	args_vrf(char **argv, int argc)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	while (i < argc)
	{
		j = ft_strlen(argv[i]);
		k = 0;
		while (k <= j)
		{
			if (argv[i][0] == '\0')
			{
				write(2, "Error\n", 6);
				return (1);
			}
			k++;
		}
		i++;
	}
	return (0);
}
