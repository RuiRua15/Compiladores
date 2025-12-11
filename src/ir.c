#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir.h"

// global counters
static int temp_count = 0;
static int label_count = 0;

// create the linked list
TAC *tac_create(TACOp op, char *result, char *arg1, char *arg2)
{
    TAC *t = (TAC *)malloc(sizeof(TAC));
    t->op = op;
    t->result = result ? strdup(result) : NULL;
    t->arg1 = arg1 ? strdup(arg1) : NULL;
    t->arg2 = arg2 ? strdup(arg2) : NULL;
    t->next = NULL;
    return t;
}

// simple list join
TAC *tac_join(TAC *l1, TAC *l2)
{
    if (!l1)
        return l2;
    if (!l2)
        return l1;

    TAC *current = l1;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = l2;
    return l1;
}

char *make_temp()
{
    char buffer[20];
    sprintf(buffer, "$t%d", temp_count++);
    return strdup(buffer);
}

char *make_label()
{
    char buffer[20];
    sprintf(buffer, "L%d", label_count++);
    return strdup(buffer);
}

// print the list
void tac_print(TAC *head)
{
    TAC *current = head;
    while (current != NULL)
    {
        switch (current->op)
        {
        case TAC_ADD:
            printf("%s = %s + %s\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_SUB:
            printf("%s = %s - %s\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_MUL:
            printf("%s = %s * %s\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_DIV:
            printf("%s = %s / %s\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_REM:
            printf("%s = %s %% %s\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_EQ:
            printf("%s = (%s == %s)\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_NEQ:
            printf("%s = (%s != %s)\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_LT:
            printf("%s = (%s < %s)\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_LEQ:
            printf("%s = (%s <= %s)\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_GT:
            printf("%s = (%s > %s)\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_GEQ:
            printf("%s = (%s >= %s)\n", current->result, current->arg1, current->arg2);
            break;
        case TAC_COPY:
            printf("%s = %s\n", current->result, current->arg1);
            break;
        case TAC_LABEL:
            printf("%s:\n", current->result);
            break;
        case TAC_GOTO:
            printf("goto %s\n", current->result);
            break;
        case TAC_IFZ:
            printf("if %s == 0 goto %s\n", current->arg1, current->result);
            break;
        case TAC_PRINT:
            printf("print %s\n", current->arg1);
            break;
        case TAC_READ:
            printf("read %s\n", current->result);
            break;
        case TAC_RET:
            printf("return\n");
            break;
        }
        current = current->next;
    }
}
