/*
    Maybe this file contains the code to have some dummy contacts so that the record appears pre filled
*/
#ifndef __POPULATE_H__
#define __POPULATE_H__

#include "contact.h"

// i have just kept them in sorted order of names...
Contact dummy_contacts[] = {
                                { .name = "Mohit Sonawane", .phone = "7890234516", .email = "mohit53@example.com" },
                                { .name = "Alice Smith", .phone = "0987654321", .email = "alice@example.com" },
                                { .name = "Bob Johnson", .phone = "1112223333", .email = "bob@company.com" },
                                { .name = "Carol White", .phone = "4445556666", .email = "carol@company.com" },
                                { .name = "David Brown", .phone = "7778889999", .email = "david@example.com" },
                                { .name = "Eve Davis", .phone = "6665554444", .email = "eve@example.com" },
                                { .name = "Frank Miller", .phone = "3334445555", .email = "frank@example.com" },
                                { .name = "Grace Wilson", .phone = "2223334444", .email = "grace@example.com" },
                                { .name = "Hannah Clark", .phone = "5556667777", .email = "hannah@example.com" },
                                { .name = "Ian Lewis", .phone = "8889990000", .email = "ian@example.com" },
                                { .name = "John Doe", .phone = "1234567890", .email = "john@example.com" }
                           };

#endif
