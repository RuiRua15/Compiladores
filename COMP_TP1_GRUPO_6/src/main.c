#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <ficheiro.adb>\n", argv[0]);
        return 1;
    }

    FILE *source = fopen(argv[1], "r");
    if (!source) {
        perror("Erro ao abrir ficheiro");
        return 1;
    }

    // Inicializar o lexer
    lexer_init(source);

    printf("=== A construir AST ===\n");

    // Chamar o parser para analisar o programa Ada
    ASTNode *program = parse_program();

    if (program) {
        printf("\n=== AST resultante ===\n");
        print_ast(program, 0);
        free_ast(program);
    } else {
        printf("Erro ao analisar o programa.\n");
    }

    fclose(source);
    return 0;
}
