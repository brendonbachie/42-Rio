#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "push_swap.h"

typedef struct s_elements
{
	int number;
	struct s_elements *next;
} t_elements;

t_elements	*ft_lstnew(int content)
{
	t_elements	*new;
	
	new = malloc(sizeof(t_elements));
	if (!new)
		return (NULL);
	new->number = content;
	new->next = NULL;
	
	return (new);
}

int main(int argc, char **argv)
{
	int a = 1;
	t_elements *head = NULL;
	t_elements *tail = NULL;
	
	while (a < argc)
	{
		if (ft_errors(argc, argv[a]) < "1")
		{
			printf("%s", "Error");
			return (-1);
		}
		a++;
	
	}
	a = 1;
	while (a < argc)
	{
		t_elements *new_element = ft_lstnew(atoi(argv[a]));
        	if (head == NULL)
        	{
        	    head = new_element;
        	    tail = new_element;
        	}
        	else
        	{
        	    tail->next = new_element;
        	    tail = new_element;
        	}
        	a++;
    	}
    	tail = head;
    	while (tail != NULL)
    	{
        	printf("%d\n", tail->number);
      	  tail = tail->next;
    	}
	return(0);
}


