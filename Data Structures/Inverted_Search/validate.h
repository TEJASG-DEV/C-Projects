#ifndef __VALIDATE_H__
#define __VALIDATE_H__
#include "list.h"

int is_valid_file(char *file_name);
int file_exists(char *file_name, FILE **handle);
int file_has_content(FILE **file);
int parse_args(Slist **head, int argc, char *argv[]);

#endif
