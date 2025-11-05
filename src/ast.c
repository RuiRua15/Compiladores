// AST constructor functions

#include <stdlib.h> // for malloc
#include <string.h>
#include "ast.h" // AST header

// EXPRESSION CONSTRUCTORS

Expr *ast_integer(int v)
{
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr *ast_variable(char *name)
{
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_VARIABLE;
  node->attr.name = strdup(name);
  return node;
}

Expr *ast_getline()
{
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_GET_LINE;
  return node;
}

Expr *ast_operation(int operator, Expr *left, Expr *right)
{
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

// STATEMENT CONSTRUCTORS

Stmt *ast_assign(char *name, Expr *value)
{
  Stmt *node = (Stmt *)malloc(sizeof(Stmt));
  node->kind = STMT_ASSIGN;
  node->attr.assign.name = strdup(name);
  node->attr.assign.value = value;
  return node;
}

Stmt *ast_if(Expr *cond, Stmt *then_body, Stmt *else_body)
{
  Stmt *node = (Stmt *)malloc(sizeof(Stmt));
  node->kind = STMT_IF;
  node->attr.if_stmt.condition = cond;
  node->attr.if_stmt.then_body = then_body;
  node->attr.if_stmt.else_body = else_body;
  return node;
}

Stmt *ast_while(Expr *cond, Stmt *body)
{
  Stmt *node = (Stmt *)malloc(sizeof(Stmt));
  node->kind = STMT_WHILE;
  node->attr.while_loop.condition = cond;
  node->attr.while_loop.body = body;
  return node;
}

Stmt *ast_putline(Expr *arg)
{
  Stmt *node = (Stmt *)malloc(sizeof(Stmt));
  node->kind = STMT_PUTLINE;
  node->attr.put_line.argument = arg;
  return node;
}

Stmt *ast_compound(Stmt *head, Stmt *next)
{
  Stmt *node = (Stmt *)malloc(sizeof(Stmt));
  node->kind = STMT_COMPOUND;
  node->attr.compound.head = head;
  node->attr.compound.next = next;
  return node;
}
