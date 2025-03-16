#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100

typedef enum {
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    UNKNOWN
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;

void initializeLexer(const char* filename); // you may need a global file pointer here (within the lexer.c)
Token getNextToken(); // Get the next token from the file. Then possibly run the categorize on it.
void categorizeToken(Token* token); // Assign TokenType within this token. Using the below isfunctions().
int isKeyword(const char* str);
int isOperator(const char* str);
int isSpecialCharacter(char ch);
int isConstant(const char* str);
int isIdentifier(const char* str);
void close_file();

#endif
