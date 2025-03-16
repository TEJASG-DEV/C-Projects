#include <stdio.h>
#include <string.h>
#include "file.h"

// Emertxe definitions
void saveContactsToFile(AddressBook *addressBook) {
    FILE *contacts_file = NULL;
    contacts_file = fopen("./contacts.csv", "w");
    
    if ( contacts_file == NULL ) {
        printf("Save : Failed to open the file.\n");
        return;
    } else {

        fprintf(contacts_file, "#%d\n", addressBook->contactCount); 
        for (int i = 0; i < addressBook->contactCount; i++) {
        
            fprintf(contacts_file, "%s,%s,%s\n",
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);

        }

        fclose(contacts_file);
        contacts_file = NULL;
    }

}

void loadContactsFromFile(AddressBook *addressBook) {
    FILE *contacts_file = NULL;
    contacts_file = fopen("./contacts.csv", "r");
    char buf[120] = { 0 }; // worst case sized buffer to hold a full line input of the file
    char *tok = NULL;
    
    if ( contacts_file == NULL ) {
        printf("Load : Failed to open the file.\n");
        return;
    } else { // reading from a corrupted file can get you screwed. No failsafe... you may figure out return value of fscanf
        fscanf(contacts_file, "#%d\n", &(addressBook->contactCount));
        
        // tried with fscanf... but it just refused to work
        // thereby used the old reliable friend fgets(). does the job perfectly.
        // reading string, then extract the tokens since the delim is a comma.
        for (int i = 0; i < addressBook->contactCount; i++) {
            fgets(buf, 120, contacts_file);
            // memcpy looks bad here
            tok = strtok(buf, ",\n");
            memset(addressBook->contacts[i].name, 0, 50);
            memcpy(addressBook->contacts[i].name, tok, strlen(tok));
            
            tok = strtok(NULL, ",\n");
            memset(addressBook->contacts[i].phone, 0, 20);
            memcpy(addressBook->contacts[i].phone, tok, strlen(tok));
            
            tok = strtok(NULL, ",\n");
            memset(addressBook->contacts[i].email, 0, 50);
            memcpy(addressBook->contacts[i].email, tok, strlen(tok));
        }

        fclose(contacts_file);
        contacts_file = NULL;
    }

}
