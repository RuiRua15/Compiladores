#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum {
    TOK_EOF = 0,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_ASSIGN,
    TOK_SEMICOLON,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_WHILE,
    TOK_LOOP,
    TOK_BEGIN,
    TOK_END,
    TOK_PROCEDURE,
    TOK_VAR,
    TOK_PUT_LINE,
    TOK_GET_LINE,
    TOK_OP_PLUS,
    TOK_OP_MINUS,
    TOK_OP_MUL,
    TOK_OP_DIV,
    TOK_OP_LT,
    TOK_OP_GT,
    TOK_OP_EQ,
    TOK_IS,
    TOK_COLON,
    TOK_INTEGER_TYPE,
    TOK_UNKNOWN
} TokenType;


typedef struct {
    TokenType type;
    char lexeme[64];
} Token;

void lexer_init(FILE *input);
Token lexer_next_token(void);
const char* token_to_string(TokenType type);

#endif
