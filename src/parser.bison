%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);

extern Stmt* root;
}

// token declarations

%token <intValue> INT
%token PROCEDURE MAIN IS TOK_BEGIN END
%token IF THEN ELSE
%token WHILE LOOP
%token PUT_LINE GET_LINE
%token <strValue> IDENTIFIER
%token ASSIGN SEMICOLON LPAREN RPAREN
%token PLUS MINUS MUL DIV  // operations
%token LT GT EQ            // bool operations

//define types -> define each grammar symbol

%union {
  int intValue;
  char* strValue;
  struct _Expr* exprValue;
  struct _Stmt* stmtValue;
}

// assign types to tokens


//assign types to grammar rules

%type <exprValue> expr term factor
%type <stmtValue> program stmt stmt_list assign_stmt if_stmt while_stmt putline_stmt

//operator precedence

%left PLUS MINUS
%left MUL DIV
%left LT GT EQ //boolean ops

%start program

%%

//grammar rules

program:
  PROCEDURE MAIN IS TOK_BEGIN stmt_list END MAIN SEMICOLON
  {
    root = $5;
    printf("Rule: Matched Program!\n");
  };

stmt_list:
  /* empty */     { $$ = NULL; }
  |
  stmt_list stmt  { $$ = ast_compound($2, $1); } // linked list
  ;

stmt:
  assign_stmt   { $$ = $1; }
  |
  if_stmt       { $$ = $1; }
  |
  while_stmt    { $$ = $1; }
  |
  putline_stmt  { $$ = $1; }
  ;

assign_stmt:
  IDENTIFIER ASSIGN expr SEMICOLON
  { $$ = ast_assign($1, $3); }
  ;
  
putline_stmt:
  PUT_LINE LPAREN expr RPAREN SEMICOLON
  { $$ = ast_putline($3); }
  ;

if_stmt:
  IF expr THEN stmt_list END IF SEMICOLON
  { $$ = ast_if($2, $4, NULL); } // sem else
  |
  IF expr THEN stmt_list ELSE stmt_list END IF SEMICOLON
  { $$ = ast_if($2, $4, $6); } // com else
  ;

while_stmt:
  WHILE expr LOOP stmt_list END LOOP SEMICOLON
  { $$ = ast_while($2, $4); }
  ;

// expression rules

expr: 
  term                { $$ = $1; }
  |
  expr PLUS term      { $$ = ast_operation(PLUS, $1, $3); }
  |
  expr MINUS term     { $$ = ast_operation(MINUS, $1, $3); }
  |
  expr LT term        { $$ = ast_operation(LT, $1, $3); }
  |
  expr GT term        { $$ = ast_operation(GT, $1, $3); }
  |
  expr EQ term        { $$ = ast_operation(EQ, $1, $3); }
  ;

term:
  factor              { $$ = $1; }
  |
  term MUL factor     { $$ = ast_operation(MUL, $1, $3); }
  |
  term DIV factor     { $$ = ast_operation(DIV, $1, $3); }
  ;
  
factor:
  INT                 { $$ = ast_integer($1); }
  |
  IDENTIFIER          { $$ = ast_variable($1); }
  |
  GET_LINE            { $$ = ast_getline(); }
  |
  LPAREN expr RPAREN  { $$ = $2; } // Pass through the inner expression
  ;

%%

//funcao de erro

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

