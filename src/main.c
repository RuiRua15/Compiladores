#include <stdio.h>
#include "ast.h"
#include "parser.h"
#include "gen_code.h"
#include "mips_code.h"

Stmt *root; // global variable for AST

void print_expr(Expr *expr, int level);
void print_stmt(Stmt *stmt, int level);
void print_op(int op);
void print_indent(int level);
extern FILE *yyin;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <input_file.adb>\n", argv[0]);
		return 1;
	}
	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		perror("Error opening file");
		return 1;
	}

	yyin = f; // read from this file instead of stdin

	printf("=== Parsing Program===\n");
	if (yyparse() == 0 && root)
	{
		printf("Parsing complete!\n");
		printf("=== Abstract Syntax Tree ===\n");
		printf("\n");
		print_stmt(root, 0);
		printf("\n");
		printf("=== Intermediate Code (TAC) ===\n");
		TAC *ir = generate_code(root);
		printf("\n");
		tac_print(ir);
		mips_generate(ir, "output.asm");
	}
	else
	{
		printf("Parsing failed\n");
	}
	fclose(f);
	return 0;
}

void print_indent(int level)

{
	for (int i = 0; i < level; i++)
	{
		printf("  ");
	}
}

void print_op(int op)
{
	switch (op)
	{
	case PLUS:
		printf("PLUS\n");
		break;
	case MINUS:
		printf("MINUS\n");
		break;
	case MUL:
		printf("MUL\n");
		break;
	case DIV:
		printf("DIV\n");
		break;
	case LT:
		printf("LT\n");
		break;
	case GT:
		printf("GT\n");
		break;
	case EQ:
		printf("EQ\n");
		break;
	case NEQ:
		printf("NEQ\n");
		break;
	case LEQ:
		printf("LEQ\n");
		break;
	case GEQ:
		printf("GEQ\n");
		break;
	case REM:
		printf("REM\n");
		break;
	case MOD:
		printf("MOD\n");
		break;
	default:
		printf("UNKNOWN_OP\n");
	}
}

void print_expr(Expr *expr, int level)
{
	if (!expr)
		return;
	print_indent(level);
	switch (expr->kind)
	{
	case E_INTEGER:
		printf("INT: %d\n", expr->attr.value);
		break;
	case E_VARIABLE:
		printf("VAR: %s\n", expr->attr.name);
		break;
	case E_GET_LINE:
		printf("GET_LINE\n");
		break;
	case E_OPERATION:
		printf("OP: ");
		print_op(expr->attr.op.operator);
		print_indent(level);
		printf(" L:\n");
		print_expr(expr->attr.op.left, level + 1);
		print_indent(level);
		printf(" R:\n");
		print_expr(expr->attr.op.right, level + 1);
		break;
	}
}

void print_stmt(Stmt *stmt, int level)

{
	if (!stmt)
		return;
	print_indent(level);
	switch (stmt->kind)
	{
	case STMT_ASSIGN:
		printf("ASSIGN: %s\n", stmt->attr.assign.name);
		print_expr(stmt->attr.assign.value, level + 1);
		break;
	case STMT_PUTLINE:
		printf("PUT_LINE\n");
		print_expr(stmt->attr.put_line.argument, level + 1);
		break;
	case STMT_IF:
		printf("IF\n");
		print_indent(level);
		printf(" COND:\n");
		print_expr(stmt->attr.if_stmt.condition, level + 1);
		print_indent(level);
		printf(" THEN:\n");
		print_stmt(stmt->attr.if_stmt.then_body, level + 1);
		if (stmt->attr.if_stmt.else_body)
		{
			print_indent(level);
			printf(" ELSE:\n");
			print_stmt(stmt->attr.if_stmt.else_body, level + 1);
		}
		break;
	case STMT_WHILE:
		printf("WHILE\n");
		print_indent(level);
		printf(" COND:\n");
		print_expr(stmt->attr.while_loop.condition, level + 1);
		print_indent(level);
		printf(" BODY:\n");
		print_stmt(stmt->attr.while_loop.body, level + 1);
		break;
	case STMT_COMPOUND:
		print_stmt(stmt->attr.compound.next, level);
		print_stmt(stmt->attr.compound.head, level);
		break;
	}
}