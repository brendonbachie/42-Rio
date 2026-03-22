typedef struct	s_list
{
	void		*content;
	struct s_list	*next;
}			t_list;

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*aux;
	
	if (lst == NULL)
		return (NULL);
	aux = lst;
	while (aux->next != NULL)
		aux = aux->next;
	return (aux);
}
