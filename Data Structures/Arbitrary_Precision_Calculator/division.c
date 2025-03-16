#include "apc.h"
#include "common.h"

static void increment(Dlist **head, Dlist **tail)
{
    Dlist *one_h = NULL, *one_t = NULL; // A Dlist to hold the value 1.
    Dlist *res_h = NULL, *res_t = NULL; // to hold increment result.
    

    // add 1 to the result list..
    if (!*head) { // input list is empty
        dl_insert_first(head, tail, 1);
    } else {
        dl_insert_first(&one_h, &one_t, 1); // initialize the one list with 1.
        
        addition(head, tail, &one_h, &one_t, &res_h, &res_t);
        // update head and tail with res_h and res_t. free / delete the one and the previous head and tail list.
        dl_delete_list(&one_h, &one_t);
        dl_delete_list(head, tail);
        
        *head = res_h;
        *tail = res_t;
    }
    
}

int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    // Here we follow the logic of division as repeated subtraction
    // This algorithm is horribly inneficient. but gets the job done.
    // have a list to hold the subtraction result.
    Dlist *res_h = NULL, *res_t = NULL;

    // rc -> return code of subtract function.
    int rc = 0; // the count will be stored in result list.

    while (rc == 0) {
        // perform subtraction as long as borrow is not set... i.e, as long as rc == 0
        rc = subtraction(head1, tail1, head2, tail2, &res_h, &res_t);
        // res contains new value. this has to be updated in head1, tail1. before that delete the list1.
        dl_delete_list(head1, tail1);
        *head1 = res_h;
        *tail1 = res_t;
        res_h = NULL;
        res_t = NULL;

        // increment the Result list... addition with 1.
        if (rc == 0) {
            increment(headR, tailR);
        } else {
            // pass
        }
        // The below can be uncommented to see this inefficient algorithm cooking.
        //print_list(*headR);
    }

    return 0;
}
