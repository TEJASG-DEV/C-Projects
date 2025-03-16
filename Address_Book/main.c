#include <stdio.h>
#include "contact.h"

int main() {
    int choice, list_choice = -1;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book

    do {
        printf("\nAddress Book Menu:\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:
                printf("What order do you want the contacts to be sorted in?\n");
                printf("1. By Name\n2. By Phone\n3. By Email\nEnter the Choice :");
                scanf("%d", &list_choice);
                list_choice--;
                listContacts(&addressBook, list_choice);
                break;
            case 6:
                printf("Saving and Exiting...\n");
                saveContactsToFile(&addressBook);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                getchar();
        }
    } while (choice != 6);
    
       return 0;
}
