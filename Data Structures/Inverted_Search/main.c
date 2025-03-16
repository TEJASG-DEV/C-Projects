#include <stdio.h>
#include "list.h"
#include "validate.h" // already has list.h
#include "db.h"

int main(int argc, char *argv[])
{
    Slist *names = NULL;
    Database *db = NULL;
    
    int loop_continue = 1, choice = 0, create_f = 0, update_f = 0;
    char buf[20] = { 0 };

    if ( parse_args(&names, argc, argv) ) {
        // pass ... do nothing as arguments are invalid.
    } else {

        while (loop_continue) {
            printf("======Menu======\n");
            printf("1. Create Database\n");
            printf("2. Display  Database\n");
            printf("3. Search Database\n");
            printf("4. Save Database\n");
            printf("5. Update Database\n");
            printf("6. Exit\n");
    
            printf("Enter choice: ");
            scanf(" %d", &choice);
    
            switch(choice) {
                case 1:
                    if ( create_f ) { // Already Created.
                        printf("Database Already Created.\n");
                    } else if ( !create_f && update_f && names ) {
                        create_database(names, &db);
                        create_f = 1;
                        printf("Database Created Successfully.\n");
                    } else if (names) { // Both create and Update is 0
                        create_database(names, &db);
                        create_f = 1;
                        printf("Database Created Successfully.\n");
                    } else {
                        // pass
                    }
                    
                    if (!names) {
                        printf("Names List is Empty.\n");
                    } else {
                        // pass
                    }

                    break;
    
                case 2:
                    if ( create_f || update_f ) {
                        display_database(db);
                    } else {
                        printf("Database Not Created / Updated.\n");
                    }
                    break;
    
                case 3:
                    if (create_f || update_f) {
                        printf("Enter the search query: ");
                        scanf(" %s", buf);
                        search_database(buf, db);
                    } else {
                        printf("Database not created or updated.\n");
                    }
                    break;
    
                case 4:
                    if (create_f || update_f) {
                        printf("Enter the file name to be saved in (<name>.txt): ");
                        scanf(" %s", buf);
                        if (is_valid_file(buf)) {
                            FILE *db_file = fopen(buf, "w");
                            save_database(db, db_file);
                            fflush(db_file);
                            fclose(db_file);
                            printf("File saved successfully.\n");
                        } else {
                            printf("Invalid file / extension.\n");
                        }
                    } else {
                        printf("Database not created or updated.\n");
                    }
                    break;
    
                case 5:
                    if (create_f || update_f) {
                        printf("Database already exists!\n");
                    } else {
                        printf("Enter the database file name: ");
                        scanf(" %s", buf);
                        if ( update_database(&names, &db, buf) ) {
                            // pass ... Not updated.
                        } else {
                            printf("Database Updated Successfully!\n");
                            update_f = 1;
                        }
                    }
                    break;
    
                case 6:
                    loop_continue = 0;
                    break;
    
                default:
                    printf("Invalid Choice!\n");
            }
        }
        
        if (db) {
            clear_database(&db);
        } else {
            // pass
        }
    
        if (names) {
            sl_delete_list(&names);
        } else {
            // pass
        }
    
    }
    
    return 0;
}
