/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitandjoin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 19:06:04 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/02/02 19:06:04 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

char	**ft_splitandjoin(char **arguments)
{
	char	**result;
	char	*temp;
	int		i;
	int		j;

	i = 1;
	temp = ft_calloc(1, 1);
	while (arguments[i] != NULL)
	{
		result = ft_split(arguments[i], ' ');
		j = 0;
		while (result[j] != NULL)
		{
			temp = ft_strjoin(&temp, result[j]);
			if (result[j + 1] != NULL || arguments[i + 1] != NULL)
				temp = ft_strjoin(&temp, " ");
			j++;
		}
		free_matrix(result);
		i++;
	}
	result = NULL;
	result = ft_split(temp, ' ');
	free(temp);
	return (result);
}
