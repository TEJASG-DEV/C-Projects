#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[]) {
    initializeLexer(argv[1]);

    Token token;
    while ((token = getNextToken()).type != UNKNOWN) {
        printf("Token: %s, Type: %d\n", token.lexeme, token.type);
    }
    close_file();
    return 0;
}
