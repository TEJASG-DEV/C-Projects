#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

int insert_at_first(Slist **head, char *str)
{
    Slist *new = malloc( sizeof(Slist) );
    
    if (!new) { // new = NULL
        printf("insert_at_first: couldn't allocate memory.\n");
        return 1;
    } else {
        new->name = str;
        new->link = NULL; // By Default NULL
    }
    
    new->link = *head;
    *head = new;
    
    return 0;
}

int insert_at_last(Slist **head, char *str)
{
    Slist *new = malloc( sizeof(Slist) );
    Slist *temp = NULL; // temp node

    if (!new) {
        return 1;
    } else {
        new->name = str;
        new->link = NULL;
    }

    if ( !(*head) ) { // Head is NULL
        *head = new;
    } else {
        temp = *head;

        while (temp->link) { // NOT NULL
            temp = temp->link;
        }
        // reached a point where temp->link == NULL
        temp->link = new;
    }

    return 0;
}

void print_list(Slist *head)
{
    if (head == NULL)
    {
      printf("INFO : List is empty\n");
    }
    else
    {
	    while (head)
	    {
		    printf("%s -> ", head->name);
		    head = head->link;
	    }

	    printf("NULL\n");
    }
}

int find_node(Slist *head, char *str)
{
    Slist *temp = NULL;
	int flag = 0, res = 0; // res is for strcmp result

	if (!head) { // Head not NULL
	    //printf("find_node: List is empty.\n");
	    return 0;
	} else {
	    temp = head;
	    while (temp) { // while temp is not null... i need to access the last element also.

	        if ( ( res = strcmp(temp->name, str)) == 0 ) {
                flag = 1;
	            break;
	        } else {
	            // pass
	        }

	        temp = temp->link;
	    }
	}

    if (flag) {
	    return 1;
    } else {
        return 0;
    }
}

int sl_delete_list(Slist **head)
{
    Slist *temp = NULL;
    
    if ( !(*head) ) { // Passed an empty list
        printf("delete_list: List is empty.\n");
        return 1;
    } else {
        temp = *head; // the first node.
        
        while (temp) { // temp not NULL
            *head = temp->link;
            free(temp);
            temp = *head;
        }

    }

    return 0;
}

int sl_delete_element(Slist **head, char *str)
{
    int res = 0; // strcmp
    Slist *temp = NULL, *prev = NULL;
    
    if (!*head) {
        printf("delete_element : List is empty.\n");
        return 1;
    } else {
        temp = *head;
        while (temp) {
            if ( ( res = strcmp(temp->name, str) ) == 0 ) {
                if (!prev) { // prev is NULL i.e, first element.
                    *head = temp->link;
                    free(temp);
                    temp = NULL;
                } else {
                    prev->link = temp->link;
                    free(temp);
                    temp = NULL;
                }
                break; // get out of the while loop.
            } else {
                // pass;
            }
            prev = temp;
            temp = temp->link;
        }
    }

    return 0;
}
