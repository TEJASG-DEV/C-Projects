#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "db.h"
#include "validate.h" // These project requirements make the code ugly.

static int initialize_database(Database *db)
{
    for (int i = 0; i < DB_SIZE; i++) {
        db->arr[i].index = i;
        db->arr[i].mlink = NULL;
    }

    return 0;
}

static int store_word_in_hash_table(char *word, char *file_name, Database *db)
{
    int index = 0, res = 0; // res is for strcasecmp result
    char first_ch = tolower(word[0]);

    main_node *new_m = NULL, *temp_m = NULL, *prev_m = NULL; // will be used later. temp_m and temp_s are for traversal operations.
    sub_node *new_s = NULL, *temp_s = NULL, *prev_s = NULL;
    
    // Now based on the first char, we store the word in the hash table... i.e, database.
    if ( isalpha(first_ch) ) {
        index = (int)( first_ch - 'a' );
    } else if ( isdigit(first_ch) ) {
        index = 26;
    } else if ( ispunct(first_ch) ) {
        index = 27;
    } else {
        // pass (It's Not added.)
    }

    if ( db->arr[index].mlink == NULL ) { // First time for the word being added.
        
        // first allocate and initialize the main node.
        new_m = (main_node *)malloc(sizeof(main_node));
        new_m->file_count = 1;
        memcpy(new_m->word, word, 20); // This may/may not lead to bugs...
        new_m->link_main = NULL;
        // All main_node data (except sub_node link initialized).
        
        // Create and initialize the sub_node.
        new_s = (sub_node *)calloc(1, sizeof(sub_node)); // calloc to avoid valgrind's silly error.
        new_s->word_count = 1;
        memcpy(new_s->file_name, file_name, strlen(file_name)); // assumed file name < 20.
        new_s->link_snode = NULL;

        // Initialize the main node's sub_node link with this new sub_node.
        new_m->link_sub = new_s;

        // Assign the link part of the database's index (link part) to new_m.
        db->arr[index].mlink = new_m;

    } else { // Node is being added the n'th time...
        
        // First you have to traverse and compare each word in main nodes.
        temp_m = db->arr[index].mlink;
        while (temp_m) { // temp_m != NULL
            if ( ( res = strcasecmp(temp_m->word, word) ) == 0 ) {// Matched word
                // Now you need to traverse sub nodes and compare each file names
                temp_s = temp_m->link_sub;
                while (temp_s) {
                    if ( ( res = strcmp(temp_s->file_name, file_name) ) == 0 ) { // Match file name
                        ++(temp_s->word_count); // increment the word count. and finish the function.
                        goto end;
                    } else {
                        // Traverse
                        prev_s = temp_s;
                        temp_s = temp_s->link_snode;
                    }
                }
                // No match for the file. therefore create the new node and then update to previous->link_snode part.
                new_s = (sub_node *)calloc(1, sizeof(sub_node));
                new_s->word_count = 1;
                memcpy(new_s->file_name, file_name, strlen(file_name));
                new_s->link_snode = NULL;
                prev_s->link_snode = new_s;
                ++(temp_m->file_count);
                goto end;
            } else {
                // Traverse
                prev_m = temp_m;
                temp_m = temp_m->link_main;
            }
        }
        
        // No match for the word. Therefore create a new main node, sub node and update it.
        new_m = (main_node *)malloc(sizeof(main_node));
        new_m->file_count = 1;
        memcpy(new_m->word, word, 20);
        new_m->link_main = NULL;

        new_s = (sub_node *)calloc(1, sizeof(sub_node));
        new_s->word_count = 1;
        memcpy(new_s->file_name, file_name, strlen(file_name));
        new_s->link_snode = NULL;

        new_m->link_sub = new_s;
        prev_m->link_main = new_m;

    }

end:
    return 0;
}

static int add_words_from_file(char *file_name, Database *db)
{
    // You need to read the data from the file as a super string. then run strtok on it.
    // or use fscanf("%s", str_buf.);
    
    int rc = 0; // Return code of fscanf
    char word_buf[20] = { 0 }; // a buffer to store words. // WARNING: Assumed that each word size < 20 bytes.

    FILE *file = fopen(file_name, "r");
    while ( (rc = fscanf(file, "%s", word_buf)) != EOF ) {
        store_word_in_hash_table(word_buf, file_name, db);
        explicit_bzero(word_buf, 20);
    }

    fclose(file);
    
    return 0;
}

static int read_nodes_from_line(Slist **files_list, Database *db, FILE *db_file)
{
    int index = -1;
    main_node *new_m = NULL, *temp_m = NULL;
    sub_node *new_s = NULL, *prev_s = NULL;

    new_m = (main_node *)malloc(sizeof(main_node));
    fscanf(db_file, "#%d;%[^;];%d;", &index, new_m->word, &new_m->file_count);
    new_m->link_main = NULL;
    
    temp_m = db->arr[index].mlink;
    while (temp_m && temp_m->link_main) { // while current and next nodes not NULL
        temp_m = temp_m->link_main; // traverse.
    }
    
    for (int i = 0; i < new_m->file_count; i++) {
        // read the sub node data for file_count times.
        new_s = (sub_node *)malloc(sizeof(sub_node));
        fscanf(db_file, "%[^;];%d;", new_s->file_name, &new_s->word_count);
        
        // here, based on the file name retrieved, we need to delete the matching file names from input file list.
        if ( find_node(*files_list, new_s->file_name) ) {
            sl_delete_element(files_list, new_s->file_name);
        } else {
            // pass
        }

        // The below part is a bit tricky!
        if (i == 0) { // The very first node created should connect to hash table.
            
            if (!temp_m) { // arr[i].mlink is NULL so update it.
                db->arr[index].mlink = new_m;
                new_m->link_sub = new_s;
                new_s->link_snode = NULL;
            } else { // main node has traversed to the last position... so add the new main and sub nodes there.
                temp_m->link_main = new_m;
                new_m->link_sub = new_s;
                new_s->link_snode = NULL;
            }

        } else { // other than that, it should connect to the last created sub node.
            prev_s->link_snode = new_s;
            new_s->link_snode = NULL;
        }
        prev_s = new_s;
        new_s = NULL; // No traversing done here...
    }
    fscanf(db_file, "#\n");

    return 0;
}

// Publicly Visible Functions:
int create_database(Slist *head, Database **db)
{
    Slist *temp = head;
    

    Database *new_db = NULL; // pre initializes memory to 0.
    
    if (!*db) { // if database passed in already exists then it's for update.
        new_db = (Database *)malloc(sizeof(Database));
        initialize_database(new_db);
    } else {
        new_db = *db;
    }

    while (temp) {
        add_words_from_file(temp->name, new_db);
        temp = temp->link;
    }
    
    *db = new_db;
    
    return 0;
}

int display_database(Database *db)
{
    main_node *temp_m = NULL;
    sub_node *temp_s = NULL;

    if (db == NULL) {
        return 1;
    } else {
        // pass
    }

    printf("============================================================================\n");
    printf(" %-10s %-15s %-15s %-20s %-10s\n", "Index", "Word", "File Count", "File Name", "Word Count");
    printf("============================================================================\n");
    for (int i = 0; i < DB_SIZE; i++) {
        if ( db->arr[i].mlink ) {
            temp_m = db->arr[i].mlink;
            while (temp_m) {
                temp_s = temp_m->link_sub;
                for (int j = 0; j < temp_m->file_count; j++) {
                    if ( j == 0 ) {
                        printf(" %-10d %-15s %-15d", i, temp_m->word, temp_m->file_count);
                    } else {
                        printf(" %-10s %-15s %-15s", "", "", "");
                    }
                    
                    printf(" %-20s %-10d\n", temp_s->file_name, temp_s->word_count);

                    temp_s = temp_s->link_snode;
                }
                temp_m = temp_m->link_main;
                // The below if is for formatting purposes.
                if (!temp_m) {
                    // pass
                } else {
                    printf("\n");
                }

            }
            printf("============================================================================\n");
        } else {
            // pass
        }
    }

    return 0;
}

int clear_database(Database **db)
{
    main_node *del_m = NULL, *temp_m = NULL;
    sub_node *del_s = NULL, *temp_s = NULL;
    for (int i = 0; i < DB_SIZE; i++) {
        if ( (*db)->arr[i].mlink ) {
            temp_m = (*db)->arr[i].mlink;
            while (temp_m) {
                del_m = temp_m;
                temp_m = temp_m->link_main;
                // first free the del_m sub_nodes:
                temp_s = del_m->link_sub;
                for (int j = 0; j < del_m->file_count; j++) {
                    del_s = temp_s;
                    temp_s = temp_s->link_snode;
                    free(del_s);
                }
                // then free the del_m node.
                del_m->link_sub = NULL;
                free(del_m);
            }
            (*db)->arr[i].mlink = NULL;
        } else {
            // pass
        }
    }
    
    free(*db);
    *db = NULL;
    return 0;
}

int save_database(Database *db, FILE *db_file)
{
    main_node *temp_m = NULL;
    sub_node *temp_s = NULL;

    // Assumed that the file is already opened in the main.
    for (int i = 0; i < DB_SIZE; i++) {
        
        if (db->arr[i].mlink) {
            temp_m = db->arr[i].mlink;
            while (temp_m) {
                fprintf(db_file, "#%d;%s;%d;", i, temp_m->word, temp_m->file_count);
                
                temp_s = temp_m->link_sub;
                for (int j = 0; j < temp_m->file_count; j++) {
                    fprintf(db_file, "%s;%d;", temp_s->file_name, temp_s->word_count);
                    temp_s = temp_s->link_snode;
                }
                fprintf(db_file, "#\n");
                temp_m = temp_m->link_main;
            }
        } else {
            // pass
        }

    }

    return 0;
}

int search_database(char *word, Database *db)
{
    int index = -1, res = 0; // res for the strcasecmp
    char first_ch = tolower(word[0]);
    
    main_node *temp_m = NULL;
    sub_node *temp_s = NULL;

    if ( isalpha(first_ch) ) {
        index = first_ch - 'a';
    } else if ( isdigit(first_ch) ) {
        index = 26;
    } else if ( ispunct(first_ch) ) {
        index = 27;
    } else {
        return 1;
    }
    
    if ( db->arr[index].mlink ) {
        temp_m = db->arr[index].mlink;
        
        while (temp_m) {
            if ( ( res = strcasecmp(temp_m->word, word) ) == 0 ) {
                printf("Word %s is present in %d file(s).\n", temp_m->word, temp_m->file_count);
                temp_s = temp_m->link_sub;
                for (int i = 0; i < temp_m->file_count; i++) {
                    printf("In file : %s %d times.\n", temp_s->file_name, temp_s->word_count);
                    temp_s = temp_s->link_snode;
                }
            } else {
                // pass
            }
            temp_m = temp_m->link_main;
        }

    } else {
        printf("Word is not present.\n");
    }
    
    return 0;
}

int update_database(Slist **files_list, Database **db, char *db_file_name)
{
    // Here you need to Read / Load the database, then remove the file names
    // (mentioned within the database) off the files within the list. therefore only unique files exist.
    // so we need a delete_from_list function.
    
    // Assumed db_file opened in read only mode.
    
    FILE *db_file = NULL;
    char first_ch = 0, last_str[3] = { 0 }; // for the first char and the last ";#" part of the file.
    int ret = 0; // strcmp return value...

    Database *new_db = NULL;

    if (is_valid_file(db_file_name)) {
        // pass
    } else {
        printf("The file %s is an Invalid database file.\n", db_file_name);
        return 1;
    }
    
    if (file_exists(db_file_name, &db_file)) { // file exists and is opened here.
        // pass
    } else {
        printf("The file %s doesn't exist.\n", db_file_name);
        return 1;
    }
    
    // Now to check weather it has content
    if (file_has_content(&db_file)) { // file content is checked but handle gets closed here. so you may need to reopen the file.
        db_file = fopen(db_file_name, "r");
    } else {
        printf("The file %s is empty.\n", db_file_name);
        return 1;
    }

    // Now check weather it's a valid database file.
    first_ch = fgetc(db_file);
    fseek(db_file, -3, SEEK_END);
    fscanf(db_file, "%s", last_str);
    rewind(db_file);
    
    if ( ( first_ch == '#' ) && ( ( ret = strcmp(last_str, ";#") ) == 0 ) ) {
        // Database is Valid.
    } else {
        printf("The database file %s is invalid!\n", db_file_name);
        fclose(db_file);
        return 1;
    }
    // All the validity checks passed! Now to read the full database.

    new_db = (Database *)malloc(sizeof(Database));
    initialize_database(new_db);
    
    while ( !feof(db_file) ) { // potential bug...
        read_nodes_from_line(files_list, new_db, db_file);
    }
    
    *db = new_db;
    // At last, close the file.
    fclose(db_file);
    
    return 0;
}
