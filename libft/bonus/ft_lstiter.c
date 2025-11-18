/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:17:14 by bgomes-b          #+#    #+#             */
/*   Updated: 2025/11/12 10:17:15 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
typedef struct	s_list
{
	void		*content;
	struct	s_list	*next;
}			t_list;

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst)
		return ;
	while (lst != NULL)
	{
		f(lst->content);
		lst = lst->next;
	}
}
