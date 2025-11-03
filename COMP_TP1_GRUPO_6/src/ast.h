#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_WHILE,
    NODE_IF,
    NODE_EXPRESSION,
    NODE_UNKNOWN
} NodeType;

static inline const char *node_type_to_string(NodeType type) {
    switch (type) {
        case NODE_PROGRAM: return "PROGRAM";
        case NODE_DECLARATION: return "DECLARATION";
        case NODE_ASSIGNMENT: return "ASSIGNMENT";
        case NODE_WHILE: return "WHILE";
        case NODE_IF: return "IF";
        case NODE_EXPRESSION: return "EXPRESSION";
        default: return "UNKNOWN";
    }
}

typedef struct ASTNode {
    NodeType type;
    char value[64];
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;
} ASTNode;

// Funções utilitárias
ASTNode *create_node(NodeType type, const char *value);
void print_ast(ASTNode *node, int level);
void free_ast(ASTNode *node);

#endif
