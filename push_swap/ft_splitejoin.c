/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitejoin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 18:07:05 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/25 21:33:24 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

char	**ft_splitandjoin(int arg_count, char **arguments)
{
	char	**result;
	char	*temp;
	int		i;
	int		j;

	i = 1;
	temp = calloc(1, 1);
	result = NULL;
	(void)arg_count;
	while (arguments[i] != NULL)
	{
		result = ft_split(arguments[i], ' ');
		j = 0;
		while(result[j] != NULL)
		{
			temp = ft_strjoin(&temp, result[j]);
			if (result[j + 1] != NULL || arguments[i + 1] != NULL)
				temp = ft_strjoin(&temp, " ");
			j++;
		}
		free(result);
		i++;
	}
	result = NULL;
	result = ft_split(temp, ' ');
	free(temp);
	return (result);
}
