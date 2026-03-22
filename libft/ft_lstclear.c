typedef struct	s_list
{
	void		*content;
	struct s_list	*next;
}			t_list;

void	ft_lstclear(t_list **lst, void (*del)(void*))
