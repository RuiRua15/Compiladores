#ifndef __IR_H__
#define __IR_H__

// ir = Intermediate Representation
// define Three Access Code operations

typedef enum
{
    TAC_ADD,   // a = b + c
    TAC_SUB,   // a = b - c
    TAC_MUL,   // a = b * c
    TAC_DIV,   // a = b / c
    TAC_REM,   // a = b % c
    TAC_EQ,    // a = (b == c)
    TAC_NEQ,   // a = (b != c)
    TAC_LT,    // a = (b < c)
    TAC_LEQ,   // a = (b <= c)
    TAC_GT,    // a = (b > c)
    TAC_GEQ,   // a = (b >= c)
    TAC_COPY,  // a = b (Assignment)
    TAC_LABEL, // Label1:
    TAC_GOTO,  // goto L1
    TAC_IFZ,   // if z == 0 goto L1 (Conditional Jump)
    TAC_PRINT, // print a
    TAC_READ,  // read a
    TAC_RET    // return
} TACOp;

// define Three Access Code ex-> a = b + c

typedef struct TAC
{
    TACOp op;
    char *arg1;   // variable name, number, or temp (t1)
    char *arg2;   // variable name, number, or temp
    char *result; // variable name, temp, or Label (L1)
    struct TAC *next;
} TAC;

// functions to manage the list
TAC *tac_create(TACOp op, char *result, char *arg1, char *arg2);
TAC *tac_join(TAC *l1, TAC *l2); // join two lists
void tac_print(TAC *head);       // print the list

// helper to create temporary names (t1, t2...) and labels (L1, L2...)
char *make_temp();
char *make_label();

#endif