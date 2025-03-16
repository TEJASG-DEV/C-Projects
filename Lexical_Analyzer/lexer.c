#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]#"; // added '#'.

static FILE *c_file = NULL;

static int is_valid_filename(const char *file_name)
{
    char *ext = NULL;
    int len = 0;

    ext = strstr(file_name, ".c");
    
    if (ext) { // ext not NULL
        if ( ( len = strlen(ext) ) == 2 ) {
            // pass    
        } else {
            printf("Invalid .c file.\n");
            return 0;
        }
    } else {
        printf("Invalid File Extension.\n");
        return 0;
    }
    
    return 1;
}

void initializeLexer(const char *filename)
{
    
    if ( is_valid_filename(filename) ) {
        c_file = fopen(filename, "r");
        
        if ( c_file ) {
            printf("File %s Opened Successfully.\n", filename);
        } else {
            printf("Failed to open file : %s .\n", filename);
        }
    } else {
        // Error message is given by the static function above.
    }

}

void close_file()
{
    if (c_file) {
        fclose(c_file);
        c_file = NULL;
    } else {
        // pass
    }
}

// The isFunctions...
int isKeyword(const char *str)
{
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if ( strcmp(str, keywords[i]) == 0 ) {
            return 1;
        } else {
            // pass
        }
    }

    return 0;
}

int isOperator(const char *str)
{
    char *exists = NULL;
    
    if (str[0]) {
        // pass
    } else {
        return 0; // empty string.
    }

    exists = strstr(operators, str);
    
    if (!exists) {
        return 0;
    } else {
        // pass... return 1
    }

    return 1;
}

int isSpecialCharacter(char ch)
{
    char *exists = NULL;
    
    if (ch) {
        // pass
    } else {
        return 0; // ch is '\0'.
    }

    exists = strchr(specialCharacters, ch);

    if (!exists) {
        return 0;
    } else {
        // pass ... return 1;
    }

    return 1;
}

int isConstant(const char *str)
{
    int len = strlen(str), is_all_digits = 1;
    
    if (len == 0) {
        return 0;
    } else {
        // pass
    }

    // All the chars should be numbers or the first and last char == '"' only then it can be constant
    // first, validate for string, then validate for digits.

    if ( ( str[0] == '"') && str[len - 1] == '"') {
        // it is a valid constant string.
        return 1;
    } else {
        // pass;
    }

    for (int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            // pass
        } else {
            is_all_digits = 0;
        }
    }

    if (is_all_digits) {
        return 1;
    } else {
        // pass ... Return 0;
    }
    
    return 0;
}

int isIdentifier(const char *str)
{
    if (str[0]) {
        // pass
    } else {
        return 0; // empty string passed.
    }

    // the string should not be a keyword. and not start with '"' and shouldn't be all digits (i,e a constant).
    if (!isKeyword(str) && !isConstant(str) && !isSpecialCharacter(str[0]) && !isOperator(str)) {
        return 1;
    } else {
        // pass ... return 0
    }

    return 0;
}

void categorizeToken(Token *token)
{
    if ( isKeyword(token->lexeme) ) {
        token->type = KEYWORD;
    } else if ( isIdentifier(token->lexeme) ) {
        token->type = IDENTIFIER;
    } else if ( isConstant(token->lexeme) ) {
        token->type = CONSTANT;
    } else if ( isOperator(token->lexeme) ) {
        token->type = OPERATOR;
    } else if ( isSpecialCharacter(token->lexeme[0])) {
        token->type = SPECIAL_CHARACTER;
    } else {
        token->type = UNKNOWN;
    }
}

Token getNextToken()
{
    // While the character scanned is not a space, and not an operator
    // if it is even one of the operators, then add it to the lexeme
    // then determine the type of operator/special char then return the token
    // then add it to the Token lexeme.
    Token t;
    explicit_bzero(t.lexeme, MAX_TOKEN_SIZE);
    t.type = UNKNOWN;

    char ch = 0; // current char.
    int i = 0; // for the lexeme index track.
    
    if ( !c_file || feof(c_file) ) {
        return t;
    } else {
        
        while ( ( ch = fgetc(c_file) ) != EOF ) {
            // ch not in the operators or in special characters
            if ( isspace(ch) && (i == 0) ) {
                continue;
            } else if ( isspace(ch) && (i != 0)) { // space encountered between token processing
                break;
            } else {
                // pass
            }

            if ( ispunct(ch) && (i == 0) ) { // special chars and operators will be here. BUG : '#' too will be accepted.
                
                if (ch == '"') {
                    t.lexeme[i] = ch;
                    while ( (t.lexeme[++i] = fgetc(c_file)) != '"' ) { // be careful of single '"' char.
                        // pass
                    }
                } else {
                    t.lexeme[i] = ch;
                }

                break;
            } else if( ispunct(ch) && (i != 0) ) { // the special char is encountered during processing of a token and not the first time.
                // make the file pointer go back 1 char.
                fseek(c_file, -1, SEEK_CUR);
                break;
            } else {
                // pass
            }
            
            t.lexeme[i] = ch;
            i++;
        }

        // Token extracted successfully.
        
        // Now determine the token type: operator, digit, or keyword or constant...
        //printf("Before Categorize: t = %s\n", t.lexeme);
        categorizeToken(&t); // here we'll use isfunctions defined within the lexer.h file.
    }
    // The token has been categorized. therefore we'll return the token.
    return t;
}
