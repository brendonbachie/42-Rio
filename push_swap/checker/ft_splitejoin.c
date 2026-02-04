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

	i = 1;
	temp = calloc(1, 1);
	result = NULL;
	while (i < arg_count)
	{
		temp = ft_strjoin(&temp, arguments[i]);
		if (i + 1 < arg_count)
			temp = ft_strjoin(&temp, " ");
		i++;
	}
	result = ft_split(temp, ' ');
	free(temp);
	return (result);
}
