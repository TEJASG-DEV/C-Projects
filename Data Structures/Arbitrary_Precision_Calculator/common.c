#include <stdio.h>
#include <stdlib.h>
#include "apc.h"
#include "common.h"

int dl_insert_last(Dlist **head, Dlist **tail, int data)
{
    Dlist *new = (Dlist *)malloc(sizeof(Dlist));

    if (!new) { // new is NULL
        printf("dl_insert_last: Couldn't allocate memory.\n");
        return FAILURE;
    } else {
        new->next = NULL;
        new->prev = NULL;
        new->data = data;
    }

    if (!*head) { // list empty
        *head = *tail = new;
    } else {
        new->prev = *tail;
        (*tail)->next = new;
        *tail = new;
    }


    return SUCCESS;
}

int dl_insert_first(Dlist **head, Dlist **tail, int data)
{
    Dlist *new = (Dlist *)malloc(sizeof(Dlist));

    if (!new) {
        printf("dl_insert_first: Cannot allocate memory.\n");
        return FAILURE;
    } else {
        new->next = NULL;
        new->prev = NULL;
        new->data = data;
    }

    if (!*head) { // head is NULL => empty
        *head = *tail = new;
    } else {
        new->next = *head;
        (*head)->prev = new;
        *head = new;
    }

    return SUCCESS;
}

/* It is unoptimized. But considering the constraints, this is the best i could do. */
void print_list(Dlist *head)
{
	int zero = 1;
    /* Cheking the list is empty or not */
	if (head == NULL)
	{
		printf("INFO : List is empty\n");
	}
	else
	{
	    while (head)		
	    {
		    /* Printing the list */
            if ( !head->data && zero ) {
                // until first  non zero digit encountered, do not print.
            } else {
		        printf("%d", head -> data);
                zero = 0; // after first non zero digit encountered, print all digits from there onwards.
            }
		    /* Travering in forward direction */
		    head = head -> next;
	    }
        puts("");
    }
}

// function to do the cleanup...
int dl_delete_list(Dlist **head, Dlist **tail)
{
    Dlist *temp = NULL;
    
    if (!*head) {
        printf("dl_delete_list : List empty.\n");
        return FAILURE;
    } else {
        temp = *head;
        while (temp) {
            *head = temp;
            temp = temp->next;
            free(*head);
        }
        *head = *tail = NULL;
    }
    
    return SUCCESS;
}
