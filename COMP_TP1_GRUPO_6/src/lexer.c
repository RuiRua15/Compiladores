#include "lexer.h"
#include <ctype.h>
#include <string.h>

static FILE *source = NULL;

void lexer_init(FILE *input) {
    source = input;
}

static int next_char(void) {
    return fgetc(source);
}

Token lexer_next_token(void) {
    Token token;
    int c;

    do {
        c = next_char();
    } while (isspace(c));

    if (c == EOF) {
        token.type = TOK_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    if (isalpha(c)) {
        char buffer[64];
        int i = 0;
        buffer[i++] = c;
        while ((c = next_char()) != EOF && (isalnum(c) || c == '_')) {
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        ungetc(c, source);

        char upper[64];
        for (int j = 0; j <= i; j++)
            upper[j] = toupper(buffer[j]);

        if (strcmp(upper, "IF") == 0) token.type = TOK_IF;
        else if (strcmp(upper, "THEN") == 0) token.type = TOK_THEN;
        else if (strcmp(upper, "ELSE") == 0) token.type = TOK_ELSE;
        else if (strcmp(upper, "WHILE") == 0) token.type = TOK_WHILE;
        else if (strcmp(upper, "LOOP") == 0) token.type = TOK_LOOP;
        else if (strcmp(upper, "BEGIN") == 0) token.type = TOK_BEGIN;
        else if (strcmp(upper, "END") == 0) token.type = TOK_END;
        else if (strcmp(upper, "PROCEDURE") == 0) token.type = TOK_PROCEDURE;
        else if (strcmp(upper, "PUT_LINE") == 0) token.type = TOK_PUT_LINE;
        else if (strcmp(upper, "GET_LINE") == 0) token.type = TOK_GET_LINE;
        else if (strcmp(upper, "IS") == 0) token.type = TOK_IS;
        else if (strcmp(upper, "INTEGER") == 0) token.type = TOK_INTEGER_TYPE;

        else token.type = TOK_IDENTIFIER;

        strcpy(token.lexeme, buffer);
        return token;
    }

    if (isdigit(c)) {
        char buffer[64];
        int i = 0;
        buffer[i++] = c;
        while ((c = next_char()) != EOF && isdigit(c)) {
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        ungetc(c, source);

        token.type = TOK_NUMBER;
        strcpy(token.lexeme, buffer);
        return token;
    }

    switch (c) {
        case ':':
            c = next_char();
            if (c == '=') {
                token.type = TOK_ASSIGN;
                strcpy(token.lexeme, ":=");
            } else {
                ungetc(c, source);
                token.type = TOK_COLON;
                strcpy(token.lexeme, ":");
            }
            return token;
        case ';':
            token.type = TOK_SEMICOLON;
            strcpy(token.lexeme, ";");
            return token;
        case '+':
            token.type = TOK_OP_PLUS;
            strcpy(token.lexeme, "+");
            return token;
        case '-':
            token.type = TOK_OP_MINUS;
            strcpy(token.lexeme, "-");
            return token;
        case '*':
            token.type = TOK_OP_MUL;
            strcpy(token.lexeme, "*");
            return token;
        case '/':
            token.type = TOK_OP_DIV;
            strcpy(token.lexeme, "/");
            return token;
        case '<':
            token.type = TOK_OP_LT;
            strcpy(token.lexeme, "<");
            return token;
        case '>':
            token.type = TOK_OP_GT;
            strcpy(token.lexeme, ">");
            return token;
        case '=':
            token.type = TOK_OP_EQ;
            strcpy(token.lexeme, "=");
            return token;
        default:
            token.type = TOK_UNKNOWN;
            token.lexeme[0] = c;
            token.lexeme[1] = '\0';
            return token;
    }
}

const char* token_to_string(TokenType type) {
    switch (type) {
        case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_NUMBER: return "NUMBER";
        case TOK_ASSIGN: return "ASSIGN";
        case TOK_IF: return "IF";
        case TOK_THEN: return "THEN";
        case TOK_ELSE: return "ELSE";
        case TOK_WHILE: return "WHILE";
        case TOK_LOOP: return "LOOP";
        case TOK_BEGIN: return "BEGIN";
        case TOK_END: return "END";
        case TOK_PROCEDURE: return "PROCEDURE";
        case TOK_PUT_LINE: return "PUT_LINE";
        case TOK_GET_LINE: return "GET_LINE";
        case TOK_OP_PLUS: return "PLUS";
        case TOK_OP_MINUS: return "MINUS";
        case TOK_OP_MUL: return "MUL";
        case TOK_OP_DIV: return "DIV";
        case TOK_OP_LT: return "LT";
        case TOK_OP_GT: return "GT";
        case TOK_OP_EQ: return "EQ";
        case TOK_SEMICOLON: return "SEMICOLON";
        case TOK_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
