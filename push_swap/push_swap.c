#include <stdio.h>
#include <stdlib.h>

typedef struct s_elements
{
    int number;
    struct s_elements *next;
} t_elements;

int main(int argc, char **argv)
{
    int a = 1;
    t_elements *head = NULL;
    t_elements *tail = NULL;

    while (a < argc)
    {
        t_elements *new_element = malloc(sizeof(t_elements));
        new_element->number = atoi(argv[a]);
        new_element->next = NULL;
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