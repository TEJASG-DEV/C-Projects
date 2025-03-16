#ifndef __COMMON_H__
#define __COMMON_H__

#include <stddef.h> // for the useless NULL definition

int dl_insert_last(Dlist **head, Dlist **tail, int data);
int dl_insert_first(Dlist **head, Dlist **tail, int data);
void print_list(Dlist *head);

// cleanup.
int dl_delete_list(Dlist **head, Dlist **tail);

#endif
