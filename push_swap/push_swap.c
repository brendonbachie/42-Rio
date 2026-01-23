#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "push_swap.h"

int main(int argc, char **argv)
{
	char **splitejoin;
	//t_elements *head = NULL;
	//t_elements *tail = NULL;

	splitejoin = ft_splitejoin(argc, argv);
	if (ft_errors(argc, splitejoin) == 0)
	{
		printf("%s", "Error");
		return (-1);
	}
	while (*splitejoin)
	{
		printf("%s\n", *splitejoin);
		splitejoin++;
	}
	return (0);
}

	/*
	a = 0;
	while (a < argc)
	{
		t_elements *new_element = ft_lstnew(atoi(split[a]));
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
*/

