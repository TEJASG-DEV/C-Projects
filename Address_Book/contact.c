#define TYPEDEF_FUNC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

// my function definitions kept static... access only to this file functions.
#ifdef TYPEDEF_FUNC
typedef int (*get_fn_ptr)(char *, AddressBook *, int); // exists for this file only.
#endif
// get_fn_ptr is a pointer to a function that returns an int, and accepts (char *) and (AddressBook *) parameters. 

// print contact function
static void print_contact(Contact c)
{
    printf("%s\t%s\t%s\n", c.name, c.phone, c.email);
}
// END print contact function

// The Below 3 are Validation functions
static int isValidName(char *name)
{
    int len = strlen(name);
    for (int i = 0; i < len; i++) {
        if ( ( name[i] >= 'A' && name[i] <= 'Z' ) || ( name[i] >= 'a' && name[i] <= 'z' ) || ( name[i] == ' ' ) ) {
        // the above condition calls for advanced validation...
            continue;
        } else {
            return 0;
        }
    }
    // the loop has completed successfully... all are normal name characters.
    return 1;
}

static int isValidNumber(char *number)
{
   int len = strlen(number);
   if (len == 10) {
        for (int i = 0; i < len; i++) {
            if ( isdigit(number[i]) ) {
                continue;
            } else {
                return 0;
            }
        }
        // all numbers are digits at this point.
        return 1;
   } else {
        return 0;
   }
}

static int isValidEmail(char *email)
{
    int len = strlen(email);
    char *at_last_idx = email, *at_last_but_one_idx = NULL; // the index of the first '@' char encountered within the email.
    
    // check if the last matching '@' is not the first character of the email...
    while ( (at_last_idx = strchr(at_last_idx, '@')) != NULL ) {
        at_last_but_one_idx = at_last_idx;
        at_last_idx++;
    }

    char *dcom_last_idx = email, *dcom_last_but_one_idx = NULL; // the index of ".com" encountered within email.
    while ( (dcom_last_idx = strstr(dcom_last_idx, ".com")) != NULL ) {
        dcom_last_but_one_idx = dcom_last_idx;
        dcom_last_idx++;
        // here no need of such validation as the above while loop.
    }
    // The above loops work flawlessly as intended.

    // post the loop evaluation, the at_last_but_one_idx will point to the last @ of the string.
    // check weather last_but_one_idx not the same as email( the base address...)
    // also check weather the .com is the last .com within the string..
    if ( at_last_but_one_idx && dcom_last_but_one_idx ) { // assert that both are not NULL
        if ( (at_last_but_one_idx != email) && (((email + len) - dcom_last_but_one_idx) == 4) ) {
            // check that the last '@' encountered is not the very first character address...
            // and also check that the string indeed ends with .com and nothing else.
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    
}
// END Validation functions

// Get Contact Details functions
static int getContactIndexByName(char *name, AddressBook *addressBook, int start_idx)
{
    int res = 0;
    for (int i = start_idx; i < addressBook->contactCount; i++) {
        if (( res = strcmp(addressBook->contacts[i].name, name) ) == 0 ) {
            return i;
        } else {
            continue;
        }
    }
    // not found
    return -1;
}

static int getContactIndexByPhone(char *phone, AddressBook *addressBook, int start_idx)
{
    int res = 0;
    for (int i = start_idx; i < addressBook->contactCount; i++) {
        if (( res = strcmp(addressBook->contacts[i].phone, phone) ) == 0 ) {
            return i;
        } else {
            continue;
        }
    }
    // not found
    return -1;
}

static int getContactIndexByEmail(char *email, AddressBook *addressBook, int start_idx)
{
    int res = 0;
    for (int i = start_idx; i < addressBook->contactCount; i++) {
        if (( res = strcmp(addressBook->contacts[i].email, email) ) == 0 ) {
            return i;
        } else {
            continue;
        }
    }
    // not found
    return -1;
}

static void getContactIndexes(int *index_arr, int *index_count, char *str, AddressBook *addressBook, get_fn_ptr ptr)
{
    int index = -1, start_idx = 0;
    for (int i = 0; i < addressBook->contactCount; i++) { // loop runs from 0 to 99.
        if ( (index = ptr(str, addressBook, start_idx)) != -1 ) {
            index_arr[*index_count] = index;
            start_idx = (index + 1);
            (*index_count)++;
        } else {
            continue;
        }
    }
}

// END get contact details functions

// Compare contacts functions for the sorting operation
static int compareContactsByName(const void *contact1, const void *contact2)
{
    // contact 1 and contact2 are pointers to Contact. (typedef'ed with struct {name,phone,email} Contact). Refer contact.h
    // size of name is 50 chars.
    return strcmp( (const char *)(((Contact *)contact1)->name), (const char *)(((Contact *)contact2)->name));
}

static int compareContactsByPhone(const void *contact1, const void *contact2)
{
    // size of phone is 20 chars.
    return strcmp( (const char *)(((Contact *)contact1)->phone), (const char *)(((Contact *)contact2)->phone));
}

static int compareContactsByEmail(const void *contact1, const void *contact2)
{
    // size of email is 50 chars.
    return strcmp( (const char *)(((Contact *)contact1)->email), (const char *)(((Contact *)contact2)->email));
}
// END compare contact functions.

// Emertxe guy's function definitions below

void listContacts(AddressBook *addressBook, int sortCriteria) 
{
    // Sort contacts based on the chosen criteria
    // initially i'll choose to keep it simple and keep the sort criteria 0... default prints pre initialized structures..
    /*if ( sortCriteria == 0 ) {
        printf("======================================================\n");
        for (int i = 0; i < addressBook->contactCount; i++) {
            print_contact(addressBook->contacts[i]);
        }
        printf("======================================================\n");
    } else {
        printf("Invalid query!");
    }
    */
    // First make a local copy of AddressBook structure.
    AddressBook copy_book;
    memcpy(&copy_book, addressBook, sizeof(AddressBook)); // try with the assignment operator if possible.

    switch (sortCriteria) {
        case 0:
            qsort(copy_book.contacts, copy_book.contactCount, sizeof(Contact), compareContactsByName);
            break;

        case 1:
            qsort(copy_book.contacts, copy_book.contactCount, sizeof(Contact), compareContactsByPhone);
            break;

        case 2:
            qsort(copy_book.contacts, copy_book.contactCount, sizeof(Contact), compareContactsByEmail);
            break;

        default:
            printf("Invalid sort option picked.\n");
            return; // exit from function... Don't print contacts
    }
    
    printf("======================================================\n");
    for (int i = 0; i < copy_book.contactCount; i++) {
        print_contact(copy_book.contacts[i]);
    }
    printf("======================================================\n");

}

void initialize(AddressBook *addressBook) {
    FILE *f_p = NULL;

    f_p = fopen("contacts.csv", "r");

    if (f_p == NULL) {
        addressBook->contactCount = 0;
        
        // Load contacts from file during initialization if it exists
        // OR pre Initialize with some dummy contacts...
        for (int i = 0; i < (sizeof(dummy_contacts) / sizeof(Contact)); i++) {
            addressBook->contacts[i] = dummy_contacts[i]; // dummy_contacts defined in populate.h
        }
    
        addressBook->contactCount = (sizeof(dummy_contacts) / sizeof(Contact));
    } else {
        fclose(f_p);
        loadContactsFromFile(addressBook);
    }
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
	char buf[50] = { 0 }, ch;
    int is_valid_name = 0, is_valid_phone = 0, is_valid_email = 0;
    int index = -1; 
    if ( addressBook->contactCount != 100 ) {
        /* Define the logic to create a Contacts */
        printf("Enter the details:\n");
        while ( (ch = getchar()) != '\n' ); // eat all new lines first. Trick from stack overflow.
        // get the name.
        while (!is_valid_name) {
            printf("Name :");
            scanf("%49[^\n]", buf);
            is_valid_name = isValidName(buf);
            if (!is_valid_name) {
                printf("Invalid Name! Please enter the name again.\n");
                getchar();
            } else {
                continue;
            }
        }
        // if the loop has exited, that means the name is indeed a valid name.
        // copy the buffer contents to the structure member...
        memcpy(addressBook->contacts[addressBook->contactCount].name, buf, 50);
        // clear the buffer.
        memset(buf, 0, 50);
        while ( (ch = getchar()) != '\n' ); // subsequent scanf will scan previous input's newline..
        
        // get the number
        while (!is_valid_phone || !(index == -1)) {
            printf("Number :");
            scanf("%19[^\n]", buf);
            is_valid_phone = isValidNumber(buf);
            index = getContactIndexByPhone(buf, addressBook, 0);
            if ( (!is_valid_phone) || !(index == -1) ) {
                if (!is_valid_phone)
                    printf("Invalid Phone Number! Please enter the number again.\n");
                if (!(index == -1))
                    printf("Phone number already exists. Try again.\n");
                getchar();
            } else {
                continue;
            }
        }
        // copy the buffer contents to the structure member...
        memcpy(addressBook->contacts[addressBook->contactCount].phone, buf, 20); // hard coded size..
        // clear the buffer.
        memset(buf, 0, 20);
        while ( (ch = getchar()) != '\n' ); // subsequent scanf will scan previous input's newline..
        
        // get the email
        while (!is_valid_email || !(index == -1) ) {
            printf("Email :");
            scanf("%49[^\n]", buf);
            is_valid_email = isValidEmail(buf);
            index = getContactIndexByEmail(buf, addressBook, 0);
            if ( (!is_valid_email) || !(index == -1) ) {
                if (!is_valid_email)
                    printf("Invalid Email! Please enter the email again.\n");
                if (!(index == -1))
                    printf("Email Address exists. Try again.\n");
                getchar();
            } else {
                continue;
            }
        }
        // copy the buffer contents to the structure member...
        memcpy(addressBook->contacts[addressBook->contactCount].email, buf, 50);
        // clear the buffer.
        memset(buf, 0, 20);
        getchar(); // for god sake.
        // finally point at next index
        (addressBook->contactCount)++;
        printf("Contact created and added successfully!\n");
    } else {
        printf("Cannot add more contacts. Space has been exhausted. Consider deleting a few.\n");
    }
}

void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    // Here only search by name has been implemented.
    get_fn_ptr f_arr[] = { getContactIndexByName, getContactIndexByPhone, getContactIndexByEmail };
    // The above is an array of function pointers.

    char buf[50] = { 0 };
    int is_valid_choice = 0, choice = 0; // for getContactIndex
    int index_arr[MAX_CONTACTS] = { 0 }, index_count = 0;
    while (!is_valid_choice) {
        printf("What do you want to search by?\n");
        printf("1. By Name\n2. By Phone Number\n3. By Email\n");
        printf("Enter Choice: ");
        getchar();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                choice--;
                printf("\nEnter the name to search :");
                getchar();
                scanf("%49[^\n]", buf);
                is_valid_choice = 1;
                break;

            case 2:
                choice--;
                printf("\nEnter the phone number to search :");
                getchar();
                scanf("%19[^\n]", buf);
                is_valid_choice = 1;
                break;

            case 3:
                choice--;
                printf("\nEnter the email to search :");
                getchar();
                scanf("%49[^\n]", buf);
                is_valid_choice = 1;
                break;

            default:
                printf("Invalid Choice! Enter again.\n");
                getchar();
        }
    }

    getContactIndexes(index_arr, &index_count, buf, addressBook, f_arr[choice]);

    if ( index_count == 0 ) {
        printf("The requested contact has not been found.\n");
    } else {
        // get all the occurances of contact then print all of them.
        // You ight need an array to hold the indexes...
        // get all other indexes with the same name(s).

        printf("\nSearch Results:\n");
        printf("======================================================\n");
        for (int i = 0; i < index_count; i++) {
            print_contact(addressBook->contacts[ index_arr[i] ]);
        }
        printf("======================================================\n");
    }   
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    get_fn_ptr f_arr[] = { getContactIndexByName, getContactIndexByPhone, getContactIndexByEmail };
    // The above is an array of function pointers

    char buf[50] = { 0 }, ch = 0; // some code will be reused for this...
    int index = -1, valid_index = -1, index_count = 0, index_arr[MAX_CONTACTS] = { 0 };
    int is_valid_name = 0, is_valid_phone = 0, is_valid_email = 0, is_valid_choice = 0, choice = 0; // some old friends from createContact
     while (!is_valid_choice) {
        printf("What do you want to search by?\n");
        printf("1. By Name\n2. By Phone Number\n3. By Email\n");
        printf("Enter Choice: ");
        getchar();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                choice--;
                printf("\nEnter the name :");
                getchar();
                scanf("%49[^\n]", buf);
                is_valid_choice = 1;
                break;

            case 2:
                choice--;
                printf("\nEnter the phone number :");
                getchar();
                scanf("%19[^\n]", buf);
                is_valid_choice = 1;
                break;

            case 3:
                choice--;
                printf("\nEnter the email :");
                getchar();
                scanf("%49[^\n]", buf);
                is_valid_choice = 1;
                break;

            default:
                printf("Invalid Choice! Enter again.\n");
                getchar();
        }
    }

    getContactIndexes(index_arr, &index_count, buf, addressBook, f_arr[choice]);

    if ( index_count == 0 ) {
        printf("The requested contact has not been found.\n");
    } else {
        // check if multiple same names there...
        if (index_count == 1) {
             index = index_arr[0]; // the first element has the required indes
        } else {
            is_valid_choice = 0;
            while (!is_valid_choice) {
                printf("Which one do you want to edit ?\n");
                printf("======================================================\n");
                for (int i = 0; i < index_count; i++) {
                    printf("%d. ", i + 1), print_contact(addressBook->contacts[ index_arr[i] ]);
                }
                printf("======================================================\n");
                //while ( (ch = getchar()) != '\n');// for scanf weirdness.
                printf("Enter choice :");
                
                // fflush(stdin); // this is BAD!
                
                scanf(" %d", &choice); // notice the whitespace match
                if (choice >= 1 && choice <= index_count) {
                    choice--;
                    index = index_arr[choice];
                    is_valid_choice = 1;
                } else {
                    printf("Invalid choice. Try again.\n");
                    //getchar();
                }
            }
        }

        printf("Enter the new details:\n");
        while ( (ch = getchar()) != '\n' ); // eat all new lines first. Trick from stack overflow.
        // get the name.
        while (!is_valid_name) {
            printf("Name :");
            scanf("%49[^\n]", buf);
            is_valid_name = isValidName(buf);
            if (!is_valid_name) {
                printf("Invalid Name! Please enter the name again.\n");
                getchar();
            } else {
                continue;
            }
        }
        // if the loop has exited, that means the name is indeed a valid name.
        // copy the buffer contents to the structure member...
        memcpy(addressBook->contacts[index].name, buf, 50);
        // clear the buffer.
        memset(buf, 0, 50);
        getchar(); // subsequent scanf will scan previous input's newline..
        
        // get the number
        while (!is_valid_phone || !(valid_index == -1)) {
            printf("Number :");
            scanf("%19[^\n]", buf);
            is_valid_phone = isValidNumber(buf);
            valid_index = getContactIndexByPhone(buf, addressBook, 0);
            if (!is_valid_phone || !(valid_index == -1)) {
                if (!is_valid_phone)
                    printf("Invalid Phone Number! Please enter the number again.\n");
                if (!(valid_index == -1))
                    printf("Phone number already exists. Try again.\n");
                getchar();
            } else {
                continue;
            }
        }
        // copy the buffer contents to the structure member...
        memcpy(addressBook->contacts[index].phone, buf, 20); // hard coded size..
        // clear the buffer.
        memset(buf, 0, 20);
        getchar(); // subsequent scanf will scan previous input's newline..
        
        // get the email
        while (!is_valid_email || !(valid_index == -1)) {
            printf("Email :");
            scanf("%49[^\n]", buf);
            is_valid_email = isValidEmail(buf);
            valid_index = getContactIndexByEmail(buf, addressBook, 0);
            if (!is_valid_email || !(valid_index == -1)) {
                if (!is_valid_email)
                    printf("Invalid Email! Please enter the email again.\n");
                if (!(valid_index == -1))
                    printf("Email already exists. Try again.\n");
                getchar();
            } else {
                continue;
            }
        }
        // copy the buffer contents to the structure member...

        memcpy(addressBook->contacts[index].email, buf, 50);
        // clear the buffer.
        memset(buf, 0, 20);
        getchar(); // for god sake.

        printf("Contact edited successfully!\n");
       
    }
    
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    get_fn_ptr f_arr[] = { getContactIndexByName, getContactIndexByPhone, getContactIndexByEmail };
    // The above is an array of pointers to functions

    char buf[50] = { 0 };// ch = 0;
    int index = -1, is_valid_choice = 0, choice = 0, index_count = 0, index_arr[MAX_CONTACTS] = { 0 };

    while (!is_valid_choice) {
        printf("What do you want to search by?\n");
        printf("1. By Name\n2. By Phone Number\n3. By Email\n");
        printf("Enter Choice: ");
        getchar();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                choice--;
                printf("\nEnter the name :");
                getchar();
                scanf("%49[^\n]", buf);
                is_valid_choice = 1;
                break;

            case 2:
                choice--;
                printf("\nEnter the phone number :");
                getchar();
                scanf("%19[^\n]", buf);
                is_valid_choice = 1;
                break;

            case 3:
                choice--;
                printf("\nEnter the email :");
                getchar();
                scanf("%49[^\n]", buf);
                is_valid_choice = 1;
                break;

            default:
                printf("Invalid Choice! Enter again.\n");
                getchar();
        }
    }

    getContactIndexes(index_arr, &index_count, buf, addressBook, f_arr[choice]);

    if ( index_count != 0 ) {
        
        if (index_count == 1) {
            index = index_arr[0];
        } else { // > 1
            is_valid_choice = 0;
            while (!is_valid_choice) {
                printf("Which one do you want to delete ?\n");
                printf("======================================================\n");
                for (int i = 0; i < index_count; i++) {
                    printf("%d. ", i + 1), print_contact(addressBook->contacts[ index_arr[i] ]);
                }
                printf("======================================================\n");
                //while ( (ch = getchar()) != '\n');// for scanf weirdness.
                printf("Enter choice :");
                
                //fflush(stdin); // this is BAD!
                
                scanf(" %d", &choice);
                if (choice >= 1 && choice <= index_count) {
                    choice--;
                    index = index_arr[choice];
                    is_valid_choice = 1;
                } else {
                    printf("Invalid choice. Try again.\n");
                    //getchar();
                }
            }
        }

        for (int i = index; i < addressBook->contactCount; i++) {
            if (i != 99) {
                addressBook->contacts[i] = addressBook->contacts[i + 1];
            } else { // i == 99
                memset(addressBook->contacts[i].name, 0, 50);
                memset(addressBook->contacts[i].phone, 0, 20);
                memset(addressBook->contacts[i].email, 0, 50);
                break;
            }
        }
    } else { // count == 0
        printf("Contact not found.\n");
        return;
    }
    
    addressBook->contactCount--;
    printf("Contact deleted successfully.\n");
    getchar(); // for god sake.
}
