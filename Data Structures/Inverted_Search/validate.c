#include <stdio.h>
#include <string.h>
#include "validate.h"

int is_valid_file(char *file_name)
{
    int len = 0, ret_val = -1;
    char *txt = strstr(file_name, ".txt");

    if (!txt) {
        ret_val = 0;
    } else {
        len = strlen(txt);
        
        // txt != NULL and the length is 4 => the very last part only then valid
        if ( txt && (len == 4) ) {
            ret_val = 1;
        } else {
            ret_val = 0;
        }
    }
    return ret_val;
}

int file_exists(char *file_name, FILE **handle)
{
    *handle = fopen(file_name, "r");
    int ret_val = -1;
    if (!*handle) {
        //printf("File %s Doesn't Exist.\n", file_name);
        ret_val = 0;
    } else {
        ret_val = 1;
    }
    // NOTE: The Opened File Has to be Closed Later...
    return ret_val;
}

int file_has_content(FILE **file)
{
    int ret_val = -1;

    long init_pos = ftell(*file);
    long end_pos = -1;

    fseek(*file, 0, SEEK_END);
    end_pos = ftell(*file);
    
    if (init_pos == end_pos) {
        ret_val = 0;
    } else {
        ret_val = 1;
    }
    
    fclose(*file); // closed here! it was opened in the above. // (file_exists)
    *file = NULL;

    return ret_val;
}

int parse_args(Slist **head, int argc, char *argv[])
{
    FILE *test_file = NULL;
    int add_to_list = 0; // A flag weather you need to add file name to the list.

    if (argc > 1) {
        // pass
    } else {
        printf("Usage : ./a.out <file1.txt> <file2.txt> ... <filen.txt>\n");
        return 1;
    }
    
    for (int i = 1; i < argc; i++ ) {
        // if valid file
        if ( is_valid_file(argv[i]) ) {
            // if file exists
            if ( file_exists(argv[i], &test_file) ) { // Too Bad! 3 deep!
                // if file has content
                if ( file_has_content(&test_file) ) {
                    add_to_list = 1;
                } else {
                    printf("The file %s is empty.\n", argv[i]);
                }

            } else {
                printf("The file %s Doesn't exist.\n", argv[i]);
            }

        } else {
            printf("Invalid file name : %s.\n", argv[i]);
        }

        if ( add_to_list ) {
            if ( find_node(*head, argv[i]) ) {
                // if node found, pass. find_node returns non zero value.
            } else {
                insert_at_last(head, argv[i]);
                printf("The file name %s is added to the List of files.\n", argv[i]);
            }
            add_to_list = 0;
        } else {
            // pass
        }

    }

    return 0;
}
