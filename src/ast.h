
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
typedef struct _Expr
{
  enum
  {
    E_INTEGER,
    E_VARIABLE,
    E_GET_LINE,
    E_OPERATION //+,-,*,/,etc
  } kind;
  union
  {
    int value; // for integer values
    char *name;
    struct
    {
      int operator; // PLUS, MINUS, etc
      struct _Expr *left;
      struct _Expr *right;
    } op; // for binary expressions
  } attr;
} Expr;

typedef struct _Stmt // assignments, conditionals (if, while), put_line
{
  enum
  {
    STMT_ASSIGN,
    STMT_IF,
    STMT_WHILE,
    STMT_PUTLINE,
    STMT_COMPOUND
  } kind;
  union
  {
    struct
    {
      char *name;
      Expr *value;
    } assign; // assign a name and a value

    struct
    {
      Expr *condition;
      struct _Stmt *then_body;
      struct _Stmt *else_body;
    } if_stmt; // if, then, else (condition, statement, statement)

    struct
    {
      Expr *condition;
      struct _Stmt *body;
    } while_loop; // while, body (condition, statement)

    struct
    {
      Expr *argument;
    } put_line; // putline (expression)

    struct
    {
      struct _Stmt *head;
      struct _Stmt *next;
    } compound; // list of statements
  } attr;
} Stmt;

typedef struct
{
  /* data */
} Program;

// Constructor functions (see implementation in ast.c)

// for expressions

Expr *ast_integer(int v);
Expr *ast_variable(char *name);
Expr *ast_getline();
Expr *ast_operation(int operator, Expr * left, Expr *right);

// for statements

Stmt *ast_assign(char *name, Expr *value);
Stmt *ast_if(Expr *cond, Stmt *then_body, Stmt *else_body);
Stmt *ast_while(Expr *cond, Stmt *body);
Stmt *ast_putline(Expr *arg);
Stmt *ast_compound(Stmt *head, Stmt *next);

#endif
