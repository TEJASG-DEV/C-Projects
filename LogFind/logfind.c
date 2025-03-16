#include <stdlib.h>
#include <glob.h>
#include "dbg.h" // stdio and string included here.
#include "read.h"
// MAX <- This is for the test cases... post completion. serves no purpose. try it in zed's folder to compare output.
int main(int argc, char *argv[])
{
    FILE *logfind_file = fopen("./.logfind", "r");
    check( ( logfind_file != NULL ) , "fopen error: couldn't open the .logfind file.");

    glob_t glob_buf; // for the glob functionality.
    glob_buf.gl_offs = 0;

    int res = 1; // for the below strcmp.
    int diff = 0, or_logic = 0; // used to determine total number of argument strings to parse.
                // or_logic to determine if or logic to be used or and logic.

    check( (argv[1] != NULL) , "error: no arguments.");
    
    if ( ( res = strcmp(argv[1], "-o") ) ) { // if non zero, then fine. and logic is defaulted.
        diff = 1; // only exclude the ./logfind ... executable name.
    } else {
        diff = 2; // this means the -o flag is given and ./logfind and -o need to be excluded therefore a diff of 2.
        or_logic = 1;
        check( (argv[2] != NULL) , "error: no arguments." );
    }
    
    int num_words = argc - diff; // can be argc - 2 if -o specified...
    int *words_found = calloc(num_words, sizeof(int)); // if you initialize a variable length array then the compiler flags an error.
    
    char *logfind_line = NULL, *found = NULL; // found is for the first occurance within file.
    int found_in_file; // a flag wearher all argv strings were found in one of the text files.
    int glob_ret = 0; // to hold return value...

    if (or_logic) {
        found_in_file = 0; // either of the occurances will set this flag.
    } else {
        found_in_file = 1; // not found -> clear the flag.
    }
    
    FILE *text_file = NULL;
    char *text_content = NULL;

    while ( *( logfind_line = readline(logfind_file) ) != '\0' ) {
        check( (*logfind_line != '\0') , "readline error: the line appears to be empty.");
        // the below codes runs in a for loop.

        glob_ret = glob(logfind_line, GLOB_NOSORT, NULL, &glob_buf);
        check( (glob_ret == 0), "glob error: glob function returned non zero value.");
        // weird prototype for check... if !(A) -> if glob_ret == 0 -> A = 1 -> !A = 0 -> no assertion triggered.
        
        for (int i = 0; i < glob_buf.gl_pathc; i++) {
            // open the text file. (path is mentioned within the .logfind file's lines)
            text_file = fopen(glob_buf.gl_pathv[i], "r");
            // read the text file fully as a super string.
            text_content = readlines(text_file);

            for (int i = 0; i < num_words; i++) {       // i beleive the below indexing is clever. 
                if ( (found = strstr(text_content, argv[or_logic + 1 + i])) != NULL ) { // found 1 occurance...
                    words_found[i] = 1;
                } else {
                    // pass
                }
            }

            for (int i = 0; i < num_words; i++) {
                if (or_logic) {
                    found_in_file |= words_found[i];
                } else {
                    found_in_file &= words_found[i];
                }
            } 

            if (found_in_file) {
                printf("%s\n", glob_buf.gl_pathv[i]);
                for (int i = 0; i < num_words; i++) {
                    words_found[i] = 0;
                }
            } else {
                // pass .. the if else of or_logic  has to be below this scope and not within this scope.. Massive bug.
            }
    
            if (or_logic) {
                found_in_file = 0;
            } else {
                found_in_file = 1;
            }
            
            free(text_content);
            text_content = NULL;
    
            fclose(text_file);
            text_file = NULL;
        }

            free(logfind_line);
            logfind_line = NULL;

            globfree(&glob_buf);
    }

error:
    if (logfind_file) {
        fclose(logfind_file);
        logfind_file = NULL;
    } // else pass

    if (logfind_line) {
        free(logfind_line);
        logfind_line = NULL;
    } // else pass

    if (text_file) {
        fclose(text_file);
        text_file = NULL;
    } // else pass

    if (text_content) {
        free(text_content);
        text_content = NULL;
    } // else pass
    
    if (words_found) {
        free(words_found);
        words_found = NULL;
    } // else pass
    
    return 0;
}
