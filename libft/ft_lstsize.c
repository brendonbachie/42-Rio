typedef struct	s_list
{
	void		*content;
	struct	s_list	*next;
}	t_list;;

int	ft_lstsize(t_list *lst)
{
	int	count;
	t_list	*aux;
	
	if (lst == NULL)
		return (NULL);
	aux = lst;
	while (aux != NULL)
	{
		count++;
		aux = aux->next;
	}
	return (count);
}
