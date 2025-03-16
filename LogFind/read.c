#include <stdio.h>
#include <stdlib.h>
#include "read.h"
char *readline(FILE *fp)
{
    char ch = '\0';
    char *line = malloc( sizeof(char) );
    int i = 0;
    while ( ( ch = fgetc(fp) ) != '\n' && !( feof(fp) ) ) {
        line[i] = ch;
        i++;
        line = realloc(line, (i + 1) );
    }
    line[i] = '\0';

    return line;
}

char *readlines(FILE *fp)
{
    char ch = '\0';
    char *str = malloc( sizeof(char) );
    int i = 0;
    while ( ( ch = fgetc(fp) ) != EOF ) {
        str[i] = ch;
        i++;
        str = realloc(str, (i + 1));
    }
    str[i] = '\0';

    return str;
}

/*
    // This commented main can be used as a reference of how the readline 
    // function is used.
    // readline reads a line from the file and returns it as a heap string.
    // it's your job to free this memory. also assign the pointer to NULL to avoid dangling pointer.
    // the string is '\0' terminated.

    // NOTE file pointer is advanced by a line.
    // so one call to readline reads first line.
    // to read second line, you may call the function again. (as fp gets 
    // advanced)

int main()
{
    FILE *log_list = NULL;
    log_list = fopen("./.logfind", "r");
    
    char *line = NULL;
    //int i = 0;

    while ( *( line = readline(log_list) ) != '\0' ) {
        printf("%d. ", (i + 1));
        printf("%s", line);
        printf("\n");
        free(line);
        line = NULL;
        i++;
    }

    line = readline(log_list);
    printf("The line no. 1 is %s\n", line);
    free(line);
    line = NULL;

    line = readline(log_list);
    printf("The line no. 2 is %s\n", line);
    free(line);
    line = NULL;

    printf("Totally %d lines read.\n", 2);
    fclose(log_list);

    return 0;
}
*/

