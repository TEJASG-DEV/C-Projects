#include <stdio.h>
#include "apc.h"
#include "common.h"

void digit_to_list(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,char *argv[])
{    
    /* The validations for non empty strings and numeric only strings are strong and are working flawlessly.*/
    /* Here my focus is on getting the list ready.*/
    
    // First numeric argument to list.
    int i = 0;
    if ( argv[1] ) {
        while ( argv[1][i] ) {
            dl_insert_last(head1, tail1, (int)( argv[1][i] - '0' ));
            i++;
        }
    } else {
        // pass
    }

    // Second numeric argument to list.
    i = 0;
    if ( argv[3] ) {
        while ( argv[3][i] ) {
            dl_insert_last(head2, tail2, (int)( argv[3][i] - '0' ));
            i++;
        }
    } else {
        // pass
    }

}
