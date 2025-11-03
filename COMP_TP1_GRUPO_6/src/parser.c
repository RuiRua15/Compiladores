#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Token atual
static Token current_token;

// Prototótipos de funções internas
static void advance(void);


// Funções auxiliares
ASTNode *create_node(NodeType type, const char *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    strcpy(node->value, value);
    node->left = node->right = node->next = NULL;
    return node;
}

void print_ast(ASTNode *node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++) printf("  ");
    printf("%s: %s\n", node_type_to_string(node->type), node->value);

    if (node->left)  print_ast(node->left,  level + 1);
    if (node->right) print_ast(node->right, level + 1);
    if (node->next)  print_ast(node->next,  level);
}


void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->next);
    free(node);
}

// Avançar para o próximo token
static void advance(void) {
    current_token = lexer_next_token();
}

/*
static int match(TokenType expected) {
    if (current_token.type == expected) {
        advance();
        return 1;
    } else {
        printf("Erro de sintaxe: esperado %s mas encontrado '%s'\n",
               token_to_string(expected), current_token.lexeme);
        return 0;
    }
}
*/

// Parser principal (muito simplificado)
ASTNode *parse_program(void) {
    advance(); // pegar primeiro token

    // Esperar: procedure Main is ...
    if (current_token.type != TOK_PROCEDURE) {
        printf("Erro: programa deve começar com 'procedure'\n");
        return NULL;
    }
    advance(); // consumir 'procedure'

    if (current_token.type != TOK_IDENTIFIER) {
        printf("Erro: esperado nome da procedure\n");
        return NULL;
    }

    ASTNode *program = create_node(NODE_PROGRAM, current_token.lexeme);
    advance(); // consumir nome

    if (current_token.type == TOK_IS) {
        advance();
    }

    // Declaração simples (X : Integer := 0;)
    if (current_token.type == TOK_IDENTIFIER) {
        ASTNode *decl = create_node(NODE_DECLARATION, current_token.lexeme);
        advance(); // X
        if (current_token.type == TOK_COLON) {
            advance();
        }
        if (current_token.type == TOK_INTEGER_TYPE) {
            advance();
        }
        if (current_token.type == TOK_ASSIGN) {
            advance();
            if (current_token.type == TOK_NUMBER) {
                decl->left = create_node(NODE_EXPRESSION, current_token.lexeme);
                advance();
            }
        }
        program->left = decl;
        if (current_token.type == TOK_SEMICOLON)
            advance();
    }

    // Esperar 'begin'
    if (current_token.type == TOK_BEGIN) {
        advance();
    }

    // WHILE loop
    if (current_token.type == TOK_WHILE) {
        ASTNode *wh = create_node(NODE_WHILE, "while");
        advance(); // while
        if (current_token.type == TOK_IDENTIFIER) {
            wh->left = create_node(NODE_EXPRESSION, current_token.lexeme);
            advance();
        }
        if (current_token.type == TOK_OP_LT) {
            advance();
        }
        if (current_token.type == TOK_NUMBER) {
            wh->right = create_node(NODE_EXPRESSION, current_token.lexeme);
            advance();
        }

        // Esperar loop
        if (current_token.type == TOK_LOOP)
            advance();

        // Corpo: X := X + 1;
        if (current_token.type == TOK_IDENTIFIER) {
            ASTNode *assign = create_node(NODE_ASSIGNMENT, current_token.lexeme);
            advance(); // X
            if (current_token.type == TOK_ASSIGN)
                advance();
            if (current_token.type == TOK_IDENTIFIER) {
                assign->left = create_node(NODE_EXPRESSION, current_token.lexeme);
                advance();
            }
            if (current_token.type == TOK_OP_PLUS)
                advance();
            if (current_token.type == TOK_NUMBER) {
                assign->right = create_node(NODE_EXPRESSION, current_token.lexeme);
                advance();
            }
            wh->next = assign;
        }

        program->right = wh;
    }

    return program;
}
