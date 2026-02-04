/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:07:48 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/26 06:34:43 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
	push_swap(&head, &head2);
	free_matrix(splitandjoin);
	return (0);
}

void	push_swap(t_node **head, t_node **head2)
{
	if (ft_lstsize(head) <= 3)
		small_numbers(head);
	else
		algorithm(head, head2);
	ft_lstclear(head);
	ft_lstclear(head2);
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
