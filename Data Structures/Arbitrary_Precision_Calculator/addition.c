#include "apc.h"
#include "common.h"

int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    Dlist *op_node1 = *tail1, *op_node2 = *tail2; // Operand nodes
    
    int oper1 = 0, oper2 = 0, res = 0, carry = 0;

    while ( (op_node1 != NULL) || (op_node2 != NULL) ) { // until both of the nodes is null
        
        // get the operands from list.
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

        res = oper1 + oper2 + carry;

        if (res >= 10) {
            carry = res / 10;
            res %= 10;
        } else {
            carry = 0;
        }

        dl_insert_first(headR, tailR, res);
        
        // Traverse logic below...
        if ( op_node1 != NULL ) {
            op_node1 = op_node1->prev;
        } else {
            // pass
        }
        
        if ( op_node2 != NULL ) {
            op_node2 = op_node2->prev;
        } else {
            // pass
        }

    }

    if (carry) {
        dl_insert_first(headR, tailR, carry);
    } else {
        // pass
    }
   
    //print_list(*headR);

    return 0;
}
