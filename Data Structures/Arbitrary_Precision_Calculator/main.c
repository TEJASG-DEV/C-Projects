#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "apc.h"
#include "common.h" // this has to be below apc.h as declarations/implementation are defined there.

static int is_valid_number(const char *str)
{
    int len = strlen(str);
    if ( len == 0 ) {
        return 0;
    } else {
        for (int i = 0; i < len; i++) {
            if ( isdigit(str[i]) ) {
                // pass
            } else {
                return 0;
            }
        }
    }

    return 1;
}

static int is_valid_operator(const char ch)
{
    if ( (ch == '+') || (ch == '-') || (ch == 'x') || (ch == '/') ) {
        return 1;
    } else {
        return 0;
    }
}

static void remove_zeros(char **arg1, char **arg2)
{
    /* In order to understand this function, refer the code within test folder.*/

    int i = 0;
    while ( (*arg1)[i] == '0' ) {
        i++;
    }

    *arg1 += i;
    
    i = 0;
    while ( (*arg2)[i] == '0' ) {
        i++;
    }

    *arg2 += i;
}

int main(int argc, char *argv[])
{
	/* Declare the pointers */
      
	Dlist *head1=NULL, *tail1=NULL;
    Dlist *head2=NULL, *tail2=NULL; 
    Dlist *headR=NULL,*tailR=NULL;


    int len1 = 0, len2 = 0, res = 0; // res is for strcmp used in subtraction.
    // len1, len2 for the argments.


    if (argc == 4) {
    
    	char operator=argv[2][0];
    
        if ( is_valid_operator(operator) ) {
    
            if ( is_valid_number(argv[1]) && is_valid_number(argv[3]) ) {
    
                switch (operator)
                {
                	case '+':
                        /* Make a list using the number. */
                        // remove all leading zeros. this will avoid most of the nuisance.
                        remove_zeros(&argv[1], &argv[3]);
                        len1 = strlen(argv[1]);
                        len2 = strlen(argv[3]);
                        if (len1 == 0 && len2 == 0) {
                            printf("0\n");
                            return 0;
                        } else {
                            // pass
                        }
                        digit_to_list(&head1,&tail1,&head2,&tail2,argv);
                        /* Add the numbers in list format. */
                        addition(&head1,&tail1,&head2,&tail2,&headR,&tailR);
                        print_list(headR);
                		break;
                	case '-':	
                        // If the number starts with a 0, then we must be truncating it. that is removing the zeroes.
                        remove_zeros(&argv[1], &argv[3]);
                        
                        // Now if len(first) > len(second), => num1 > num2. else if numbers are equal, use strcmp to 
                        // deduce weather equal or not. if exactly equal, then just print 0.

                        len1 = strlen(argv[1]);
                        len2 = strlen(argv[3]);

                        digit_to_list(&head1, &tail1, &head2, &tail2, argv);

                        /* First determine which of the numbers is greater than the other. */
                        if (len1 > len2) {
                            // num1 > num2. result positive.
                            // just proceed with normal subtraction.
                            subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);
                            print_list(headR);
                        } else if (len1 < len2) {
                            // num1 < num2. result negative.
                            // print a '-' then proceed with normal subtraction on swapped arguments.
                            printf("-");
                            subtraction(&head2, &tail2, &head1, &tail1, &headR, &tailR);
                            print_list(headR);
                        } else { // equal.
                            // here we need the strcmp
                            res = strcmp(argv[1], argv[3]);
                            if ( res > 0 ) { // num1 > num2.
                                subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);
                                print_list(headR);
                            } else if ( res < 0 ) { // num1 < num2.
                                printf("-");
                                subtraction(&head2, &tail2, &head1, &tail1, &headR, &tailR);
                                print_list(headR);
                            } else { // res == 0, num1 == num2.
                                printf("0\n");
                            }
                        }

                		break;
                	case 'x':	
                		/* call the function to perform the multiplication operation */
                        remove_zeros(&argv[1], &argv[3]);
                        len1 = strlen(argv[1]);
                        len2 = strlen(argv[3]);
                        
                        if ( (len1 == 0) || (len2 == 0) ) { // Actual zeros passed
                            printf("0\n");
                        } else {
                            digit_to_list(&head1, &tail1, &head2, &tail2, argv);
                            multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR);
                            print_list(headR);
                        }

                		break;
                	case '/':	
                		/// If the number starts with a 0, then we must be truncating it. that is removing the zeroes.
                        remove_zeros(&argv[1], &argv[3]);
                        
                        // Now if len(first) > len(second), => num1 > num2. else if numbers are equal, use strcmp to 
                        // deduce weather equal or not. if exactly equal, then just print result accordingly.

                        len1 = strlen(argv[1]);
                        len2 = strlen(argv[3]);

                        digit_to_list(&head1, &tail1, &head2, &tail2, argv);

                        /* First determine which of the numbers is greater than the other. */
                        if (len1 > len2) {
                            // num1 > num2. division is possible provided num2 not 0.
                            if (len2 == 0) {
                                printf("Divide By 0 Caught!\n");
                            } else {
                                division(&head1, &tail1, &head2, &tail2, &headR, &tailR);
                                print_list(headR);
                            } 
                        } else if (len1 < len2) {
                            // num1 < num2. result = 0
                            printf("0\n");
                        } else { // equal.
                            // here we need the strcmp
                            res = strcmp(argv[1], argv[3]);
                            if ( res > 0 ) { // num1 > num2.
                                // Division possible provided num2 not = 0.
                                if (len2 == 0) {
                                    printf("Divide By 0 Caught!\n");
                                } else {
                                    division(&head1, &tail1, &head2, &tail2, &headR, &tailR);
                                    print_list(headR);
                                }
                            } else if ( res < 0 ) { // num1 < num2.
                                printf("0\n");
                            } else { // res == 0, num1 == num2.
                                printf("1\n");
                            }
                        }

                		break;
                	default:
                		printf("Invalid Input :-( Try again...\n");
                }
                
                // cleanup...
                dl_delete_list(&head1, &tail1);
                dl_delete_list(&head2, &tail2);
                dl_delete_list(&headR, &tailR);
    
            } else {
                printf("Invalid Operands!\n");
            }
    
        } else {
            printf("Invalid Operator!\n");
        }
        
    } else {
        printf("Usage : ./a.out <operand1> operator(+,-,x,/) <operand2>\n");
    }
	return 0;
}
