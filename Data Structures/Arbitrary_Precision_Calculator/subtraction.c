#include <stdio.h>
#include "apc.h"
#include "common.h"

int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    Dlist *op_node1 = *tail1, *op_node2 = *tail2;
    int oper1 = 0, oper2 = 0, borrow = 0, res = 0;
    
    //printf("============Before while : ==============\n");
    while ( ( op_node1 != NULL ) || ( op_node2 != NULL ) ) {
        
        // extract the operands
        if ( op_node1 == NULL ) {
            oper1 = 0;
        } else {
            oper1 = op_node1->data;
        }

        if ( op_node2 == NULL ) {
            oper2 = 0;
        } else {
            oper2 = op_node2->data;
        }
        
        // perform the subtraction.
        if (oper1 <= oper2) {
            
            if (oper1 < oper2) {
                res = ( 10 + (oper1 - borrow) ) - oper2;
                borrow = 1;
            } else { // equal
                if (borrow) {
                    res = ( 10 + (oper1 - borrow) ) - oper2;
                    borrow = 1;
                } else {
                    res = 0; // equal => difference = 0
                    borrow = 0;
                }
            }

        } else {
            
            res = (oper1 - borrow) - oper2;
            borrow = 0;

        }
        //printf("oper1 = %d, oper2 = %d, res = %d\n", oper1, oper2, res);
        // Add the result to the list.
        dl_insert_first(headR, tailR, res);

        // Traversal logic below...
        if ( op_node1 == NULL ) {
            // pass
        } else {
            op_node1 = op_node1->prev;
        }

        if ( op_node2 == NULL ) {
            // pass
        } else {
            op_node2 = op_node2->prev;
        } 

    }
    //printf("==========After While: Borrow = %d===========\n", borrow);
    if (borrow) { // this added later. for the division to work.
        return 1;
    } else {
        // pass
    }

       
    //print_list(*headR);

    return 0;
}
