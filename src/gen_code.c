// Logica para transformar ast stmt list em tac list -> Divide and Conquer, processar os filhos e usar os resultados para subir na arvore
// para expressoes é simples -> (a+b)*c, visitamos o no superior, i.e. *, tem dois filhos (a+b) e c, o da esquerda fazemos t1 = a+b e c,
// subimos na arvore para o no inicial * e t2 = t1*c, retorna t2 ($t1 = a + b , $t2 = $t1 * c)
// para assignments, por exemplo x := 10 + 5; chamamos gen_expr(10+5) e isto retorna t1->(10+5), fazemos TAC_COPY -> x = t1
// !!!! if e while pensar nas blueprints antes de fazer P.S. meter este comentário de forma bonita no relatorio

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gen_code.h"
#include "symtab.h"
#include "parser.h"

// helper function to expr_op -> tac_op
TACOp get_tac_op(int ast_op)
{
    switch (ast_op)
    {
    case PLUS:
        return TAC_ADD;
    case MINUS:
        return TAC_SUB;
    case MUL:
        return TAC_MUL;
    case DIV:
        return TAC_DIV;
    case REM:
        return TAC_REM;
    case MOD:
        return TAC_REM; // treating mod same as rem for now
    case LT:
        return TAC_LT;
    case LEQ:
        return TAC_LEQ;
    case GT:
        return TAC_GT;
    case GEQ:
        return TAC_GEQ;
    case EQ:
        return TAC_EQ;
    case NEQ:
        return TAC_NEQ;
    default:
        return TAC_ADD; // default, nao sei o q meter
    }
}

// Expr generator
// returns the name of the variable ( or temp ) that holds the result ex: prints t1=10, returns t1
char *gen_expr(Expr *e, TAC **list)
{
    if (!e)
        return NULL;

    if (e->kind == E_INTEGER)
    {
        char *temp = make_temp();
        char num_str[20];
        sprintf(num_str, "%d", e->attr.value);
        // t1= 10
        TAC *inst = tac_create(TAC_COPY, temp, num_str, NULL);
        *list = tac_join(*list, inst);
        return temp;
    }

    if (e->kind == E_VARIABLE)
    {
        return strdup(e->attr.name);
    }

    if (e->kind == E_GET_LINE)
    {
        char *temp = make_temp();
        // read t1
        TAC *inst = tac_create(TAC_READ, temp, NULL, NULL);
        *list = tac_join(*list, inst);
        return temp;
    }
    if (e->kind == E_OPERATION)
    {
        // generate both left and right and make temp for result
        char *res1 = gen_expr(e->attr.op.left, list);
        char *res2 = gen_expr(e->attr.op.right, list);
        char *temp = make_temp();

        // create the instance, ex: t1 = a + b, or in this case temp = res1 op res2.
        TACOp op = get_tac_op(e->attr.op.operator);
        TAC *inst = tac_create(op, temp, res1, res2);
        *list = tac_join(*list, inst);
        return temp;
    }
    return NULL;
}

// stmt generator
// returns the list of instructions for this stmt
//
TAC *gen_stmt(Stmt *s)
{
    if (!s)
        return NULL;

    TAC *code = NULL; // list

    switch (s->kind)
    {
    case STMT_ASSIGN:
    {
        // generate the expression first
        char *result_name = gen_expr(s->attr.assign.value, &code);
        // generate the assignment: a = t1
        TAC *copy = tac_create(TAC_COPY, s->attr.assign.name, result_name, NULL);
        code = tac_join(code, copy);
        break;
    }

    case STMT_PUTLINE:
    {
        char *result_name = gen_expr(s->attr.put_line.argument, &code);
        // print t1
        TAC *print = tac_create(TAC_PRINT, NULL, result_name, NULL);
        code = tac_join(code, print);
        break;
    }

    case STMT_IF:
    {
        /*
        blueprints dum if

        code_cond
        ifz t1 goto Label_FALSE
        code_then
        goto Label_END
        Label_FALSE:
        code_else
        Label_END:
        */
        char *Label_false = make_label();
        char *Label_end = make_label();

        char *cond_res = gen_expr(s->attr.if_stmt.condition, &code);

        // if false ->Label_false
        TAC *jump_false = tac_create(TAC_IFZ, Label_false, cond_res, NULL);
        code = tac_join(code, jump_false);
        // then
        TAC *then_code = gen_stmt(s->attr.if_stmt.then_body);
        code = tac_join(code, then_code);
        // jump end
        TAC *jump_end = tac_create(TAC_GOTO, Label_end, NULL, NULL);
        code = tac_join(code, jump_end);
        // label_false
        TAC *inst_Label_false = tac_create(TAC_LABEL, Label_false, NULL, NULL);
        code = tac_join(code, inst_Label_false);
        // else
        if (s->attr.if_stmt.else_body)
        {
            TAC *else_code = gen_stmt(s->attr.if_stmt.else_body);
            code = tac_join(code, else_code);
        }
        // label_end
        TAC *inst_Label_end = tac_create(TAC_LABEL, Label_end, NULL, NULL);
        code = tac_join(code, inst_Label_end);
        break;
    }

    case STMT_WHILE:
    {
        /*
        blueprint dum while

        Label_START:
        code_cond
        ifz t1 goto Label_END
        code_body
        goto Label_START
        Label_END:
        */
        char *Label_start = make_label();
        char *Label_end = make_label();

        // label_start
        TAC *inst_Label_start = tac_create(TAC_LABEL, Label_start, NULL, NULL);
        code = tac_join(code, inst_Label_start);
        // cond
        char *cond_res = gen_expr(s->attr.while_loop.condition, &code);
        // if false -> label_end
        TAC *jump_end = tac_create(TAC_IFZ, Label_end, cond_res, NULL);
        code = tac_join(code, jump_end);
        // body
        TAC *body_code = gen_stmt(s->attr.while_loop.body);
        code = tac_join(code, body_code);
        // start
        TAC *jump_start = tac_create(TAC_GOTO, Label_start, NULL, NULL);
        code = tac_join(code, jump_start);
        // label end
        TAC *inst_Label_end = tac_create(TAC_LABEL, Label_end, NULL, NULL);
        code = tac_join(code, inst_Label_end);
        break;
    }
    // glue code, ler ao contratio
    case STMT_COMPOUND:
    {
        TAC *next = gen_stmt(s->attr.compound.next);
        TAC *head = gen_stmt(s->attr.compound.head);

        code = tac_join(next, head);
        break;
    }
    }

    return code;
}

// start at root, recursive down
TAC *generate_code(Stmt *root)
{
    return gen_stmt(root);
}
