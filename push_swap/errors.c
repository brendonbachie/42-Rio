/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:48:26 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 20:38:04 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_errors(char **args, t_node *stack)
{
	int		i;
	long	b;

	i = 0;
	if (ft_isnum(args) || has_duplicate(stack))
	{
		write(2, "Error\n", 6);
		return (1);
	}
	while (args[i] != NULL)
	{
		b = ft_atol(args[i]);
		if (b < INT_MIN || b > INT_MAX)
		{
			write(2, "Error\n", 6);
			return (1);
		}
		i++;
	}
	if (is_ordened(stack))
		return (1);
	return (0);
}

int	has_duplicate(t_node *stack)
{
	t_node	*a;
	t_node	*b;

	a = stack;
	while (a)
	{
		b = a->next;
		while (b)
		{
			if (a->number == b->number)
				return (1);
			b = b->next;
		}
		a = a->next;
	}
	return (0);
}

int	ft_isnum(char **str)
{
	int	i;
	int	a;

	i = 0;
	a = 0;
	while (str[i] != NULL)
	{
		while (str[i][a] == ' ')
			a++;
		if ((str[i][a] == '-' || str[i][a] == '+')
			&& !(a == 0 || str[i][a - 1] == ' '))
			return (1);
		else if (str[i][a] == '-' || str[i][a] == '+')
			a++;
		if ((str[i][a] < '0' || str[i][a] > '9'))
			return (1);
		a++;
		if (str[i][a] == '\0')
		{
			i++;
			a = 0;
		}
	}
	return (0);
}

int	is_ordened(t_node *stack_a)
{
	t_node	*temp;

	temp = stack_a;
	while (temp->next)
	{
		if (temp->number < temp->next->number)
			temp = temp->next;
		else
			return (0);
	}
	return (1);
}

void	return_error(t_node **stack, char **args)
{
	if (stack && *stack)
		ft_lstclear(stack);
	if (args)
		free_matrix(args);
}
