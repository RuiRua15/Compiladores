#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mips_code.h"
#include "symtab.h"

// para saber onde "andam" as temps (t0, t1) vamos usar uma linked list "TempMap", mais simples, apesar de menos eficiente
typedef struct TempMap
{
    char *name; // e.g., "$t0"
    int offset; // e.g., -16
    struct TempMap *next;
} TempMap;

static TempMap *temp_head = NULL;
static int current_stack_pointer = -100; // start temps below local variables (safety)

// basic helper to traverse the ll to find offset
int get_temp_offset(char *name)
{
    TempMap *curr = temp_head;
    // procurar
    while (curr)
    {
        if (strcmp(curr->name, name) == 0)
            return curr->offset;
        curr = curr->next;
    }
    // criar se nao encontrada
    TempMap *new_node = malloc(sizeof(TempMap));
    new_node->name = strdup(name);
    current_stack_pointer -= 4;
    new_node->offset = current_stack_pointer;
    new_node->next = temp_head;
    temp_head = new_node;
    return new_node->offset;
}

// offset of anything
int get_offset(char *name)
{
    // is a user variable? symtab
    Symbol *s = symbol_find(name);
    if (s)
        return s->offset;
    // if not, its a temp
    return get_temp_offset(name);
}

// is a raw number
int is_number(char *s)
{
    return (s[0] >= '0' && s[0] <= '9') || s[0] == '-';
}

// load a value into a register
void emit_load(FILE *f, char *reg, char *arg)
{
    if (is_number(arg)) // load immediate, put into reg $t0
    {
        fprintf(f, " li %s, %s \n", reg, arg);
    }
    else // load word, put into reg t0 whats in "offset" bytes below frame pointer
    {
        int offset = get_offset(arg);
        fprintf(f, " lw %s, %d($fp) \n", reg, offset);
    }
}

void emit_store(FILE *f, char *reg, char *result) // store word in reg t0 and write in "offset" bytes below fp
{
    int offset = get_offset(result);
    fprintf(f, " sw %s, %d($fp)\n", reg, offset);
}

void mips_generate(TAC *head, const char *filename)
{
    FILE *f = fopen(filename, "w");
    if (!f)
    {
        perror("Error creating MIPS file");
        return;
    }

    // --- PROLOGUE ---
    fprintf(f, ".data\n");
    fprintf(f, "newline: .asciiz \"\\n\"\n");
    fprintf(f, ".text\n");
    fprintf(f, ".globl main\n");
    fprintf(f, "main:\n");
    fprintf(f, "  # Prologue\n");
    fprintf(f, "  move $fp, $sp\n");       // set frame pointer
    fprintf(f, "  subu $sp, $sp, 1000\n"); // allocate giant stack frame (lazy)

    TAC *curr = head;
    while (curr)
    {
        fprintf(f, "\n  # %d\n", curr->op); // this creates a comment to easily identify what operation its writing

        switch (curr->op)
        {
        // OPS basicas
        case TAC_COPY: // res = arg1
            emit_load(f, "$t0", curr->arg1);
            emit_store(f, "$t0", curr->result);
            break;

        case TAC_ADD: // res = arg1 + arg2
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  add $t2, $t0, $t1\n");
            emit_store(f, "$t2", curr->result);
            break;

        case TAC_SUB: // res = arg1 - arg2
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  sub $t2, $t0, $t1\n");
            emit_store(f, "$t2", curr->result);
            break;

        case TAC_MUL:
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  mul $t2, $t0, $t1\n");
            emit_store(f, "$t2", curr->result);
            break;

        case TAC_DIV:
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  div $t0, $t1\n"); // divide
            fprintf(f, "  mflo $t2\n");     // lo = quociente
            emit_store(f, "$t2", curr->result);
            break;

        case TAC_REM:
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  div $t0, $t1\n");
            fprintf(f, "  mfhi $t2\n"); // hi = resto
            emit_store(f, "$t2", curr->result);
            break;

        // jumps e labels
        case TAC_LABEL:
            fprintf(f, "%s:\n", curr->result);
            break;

        case TAC_GOTO:
            fprintf(f, "  j %s\n", curr->result);
            break;

        case TAC_IFZ: // if arg1 == 0 goto result
            emit_load(f, "$t0", curr->arg1);
            fprintf(f, "  beqz $t0, %s\n", curr->result);
            break;

        // OPS relacionais
        // set on less than faz o trabalho todo
        case TAC_LT:
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  slt $t2, $t0, $t1\n");
            emit_store(f, "$t2", curr->result);
            break;

        // agora é combinar slt para ter as outras ops
        case TAC_GT: // a > b  =  b < a
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  slt $t2, $t1, $t0\n"); // reversed args
            emit_store(f, "$t2", curr->result);
            break;

        case TAC_LEQ: // a <= b = !(b < a)
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  slt $t2, $t1, $t0\n"); // check b < a
            fprintf(f, "  xori $t2, $t2, 1\n");  // flip 0/1
            emit_store(f, "$t2", curr->result);
            break;

        case TAC_GEQ: // a >= b = !(a < b)
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  slt $t2, $t0, $t1\n"); // check a < b
            fprintf(f, "  xori $t2, $t2, 1\n");  // flip 0/1
            emit_store(f, "$t2", curr->result);
            break;

        case TAC_EQ: // a == b
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  seq $t2, $t0, $t1\n"); // set equal
            emit_store(f, "$t2", curr->result);
            break;

        case TAC_NEQ: // a != b
            emit_load(f, "$t0", curr->arg1);
            emit_load(f, "$t1", curr->arg2);
            fprintf(f, "  sne $t2, $t0, $t1\n"); // set not equal
            emit_store(f, "$t2", curr->result);
            break;

        // IO syscall 1 -> print int, 4 -> print string, 5 -> read int, 10-> stop running
        case TAC_PRINT:
            emit_load(f, "$a0", curr->arg1);
            fprintf(f, "  li $v0, 1\n"); // print int syscall
            fprintf(f, "  syscall\n");
            // print a newline for prettiness
            fprintf(f, "  li $v0, 4\n");
            fprintf(f, "  la $a0, newline\n");
            fprintf(f, "  syscall\n");
            break;

        case TAC_READ:
            fprintf(f, "  li $v0, 5\n"); // read int syscall
            fprintf(f, "  syscall\n");
            emit_store(f, "$v0", curr->result);
            break;

        default:
            break;
        }
        curr = curr->next;
    }
    // --- EPILOGUE ---
    fprintf(f, "\n  # Epilogue\n");
    fprintf(f, "  li $v0, 10\n"); // Exit syscall
    fprintf(f, "  syscall\n");

    fclose(f);
    printf("MIPS code generated to %s\n", filename);
}