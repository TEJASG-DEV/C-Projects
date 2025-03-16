//#include <stdio.h>
#include "apc.h"
#include "common.h"
                            // result  list                       mul_res list..
static int add_sum(Dlist **net_sum_h, Dlist **net_sum_t, Dlist **mul_res_h, Dlist **mul_res_t)
{
    // Create a new result list.
    Dlist *res_h = NULL, *res_t = NULL;
    
    // Call the add function and store the result within this result list.
    addition(net_sum_h, net_sum_t, mul_res_h, mul_res_t, &res_h, &res_t);
    
    // free (or) delete the net_sum list.
    dl_delete_list(net_sum_h, net_sum_t);
    
    // now do a shallow copy of result list with net_sum.
    *net_sum_h = res_h;
    *net_sum_t = res_t;

    return 0;
}

static int len_list(Dlist **head)
{
    int len = 0;
    Dlist *temp = *head;
    while ( temp ) {
        temp = temp->next;
        ++len;
    }

    return len;
}

static void insert_zeros(Dlist **head, Dlist **tail, int count)
{
    for (int i = 0; i < count; i++) {
        dl_insert_first(head, tail, 0);
    }
}

int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	int len_longest = 0, len_lst1 = 0, len_lst2 = 0; // longest of the 2 lists.
    
    int oper1 = 0, oper2 = 0, res = 0, carry = 0;

    Dlist *long_list = NULL, *short_list = NULL, *mul_res_h = NULL, *mul_res_t = NULL, *short_tail = NULL;
    //Dlist *net_sum_h = NULL, *net_sum_t = NULL; (net_sum list is the result list.)
    len_lst1 = len_list(head1);
    len_lst2 = len_list(head2);

    //printf("List 1 Length : %d\n", len_lst1);
    //printf("List 2 Length : %d\n", len_lst2);

    
    // Initialization so that upcoming multiplication performed properly.
    if (len_lst1 > len_lst2) {
        long_list = *head1;
        //short_list = *tail2;
        short_tail = *tail2;
        len_longest = len_lst1;
    } else if ( len_lst1 < len_lst2 ) {
        long_list = *head2;
        //short_list = *tail1;
        short_tail = *tail1;
        len_longest = len_lst2;
    } else { // order doesn't matter. both lengths equal.
        long_list = *head1;
        //short_list = *tail2;
        short_tail = *tail2;
        len_longest = len_lst1; // or len_lst2
    }

    for (int i = len_longest; i > 0; i--) {
        // first append the zeros within the result list.
        insert_zeros(&mul_res_h, &mul_res_t, (i - 1));
        // make the carry 0
        carry = 0;

        /* Now the Bulk of Multiplication. */
        
        // Extract the operand1.
        oper1 = long_list->data;

        // multiply with all the operands of short list with the above oper1.
        // (re)initialize short_list with the tail.
        short_list = short_tail;
        while ( short_list ) { // while short_list not NULL
            // get the oper2.
            oper2 = short_list->data;
            // multiply and store the result. but include previous carry. (first time carry = 0)
            res = (oper1 * oper2) + carry;
            
            if (res > 9) {
                carry = res / 10;
                res %= 10;
            } else {
                carry = 0;
            }

            // store the result in mul_res list.
            dl_insert_first(&mul_res_h, &mul_res_t, res);

            // Traverse.
            short_list = short_list->prev;
        }
        // if carry non zero, then append the carry to mul_res list.
        if (carry) {
            dl_insert_first(&mul_res_h, &mul_res_t, carry);
        } else {
            // pass
        }

        // Add the mul_res list to the net_sum list
        // but if net_sum list is empty, then you need to make a shallow copy of mul_res list to the net_sum list
        // and make the mul_res list NULL.

        if (!*headR) { // net_sum (result) list empty.
            *headR = mul_res_h;
            *tailR = mul_res_t;
            mul_res_h = NULL;
            mul_res_t = NULL;
        } else { // not empty.
            // do the addition of lists.
            add_sum(headR, tailR, &mul_res_h, &mul_res_t);
            // After the above, the full sum is stored in the net_sum list.
            // delete the mul_res list as the next iteration creates a new mul_res.
            dl_delete_list(&mul_res_h, &mul_res_t); // mul_res_h and mul_res_t are made NULL by the function itself.
        }

        // Traverse the long_list to next node.
        long_list = long_list->next;
    }
        
    //print_list(*headR);

    return 0;
}
