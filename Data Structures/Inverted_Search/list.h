#ifndef __LIST_H__
#define __LIST_H__

// The data type:
typedef struct node
{
	char *name;
	struct node *link;
} Slist;


// The functions: Note : this will only need the stripped-down necessary functionality.
int insert_at_last(Slist **head, char *);
int insert_at_first(Slist **head, char *);

int find_node(Slist *head, char *);

int sl_delete_list(Slist **);
int sl_delete_element(Slist **, char *);

void print_list(Slist *head);

#endif
