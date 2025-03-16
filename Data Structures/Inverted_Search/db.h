#ifndef __DB_H__
#define __DB_H__

#include <stdio.h> // for the save_database.
#include "list.h"

#define DB_SIZE 28

// This file will contain all the database related definitions and functions.

// The sub node:
typedef struct s_node {
    int word_count;
    char file_name[20];
    struct s_node *link_snode;
} sub_node;

// The main node:
typedef struct m_node {
    int file_count;
    char word[20];
    struct m_node *link_main;
    struct s_node *link_sub;
} main_node;

// The hash table node:
typedef struct h_t_node {
    int index;
    struct m_node *mlink;
} hash_t_node;

// The Database: i.e, the array of hash_t nodes

typedef struct db {
    hash_t_node arr[DB_SIZE];
} Database;

// The functions:

int create_database(Slist *head, Database **db);
int display_database(Database *db);
int clear_database(Database **db);
int save_database(Database *db, FILE *db_file);
int search_database(char *word, Database *db);
int update_database(Slist **files_list, Database **db, char *db_file_name);

#endif
