%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtab.h"

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
%token PLUS MINUS MUL DIV REM MOD  // operations
%token LT GT EQ NEQ LEQ GEQ        // bool operations

// new tokens for declarations
%token COLON COMMA INTEGER_TYPE

//define types -> define each grammar symbol

%union {
  int intValue;
  char* strValue;
  struct _Expr* exprValue;
  struct _Stmt* stmtValue;
}

//assign types to grammar rules

%type <exprValue> expr term factor
%type <stmtValue> program stmt stmt_list assign_stmt if_stmt while_stmt putline_stmt

//operator precedence

%left PLUS MINUS
%left MUL DIV REM MOD
%left LT GT EQ NEQ LEQ GEQ //boolean ops

// root

%start program

%%

//grammar rules

program:
  PROCEDURE MAIN IS opt_decl_list TOK_BEGIN stmt_list END MAIN SEMICOLON
  {
    root = $6;
    printf("Rule: Matched Program!\n");
  };

opt_decl_list:
  |
  decl_list
  ;

decl_list:
  declaration
  |
  decl_list declaration
  ;

declaration:
  id_list COLON INTEGER_TYPE SEMICOLON
  ;

id_list:
  IDENTIFIER
  {
    // semantic check for duplicate declaration
    if(symbol_exists($1))
    {
      char errmsg[256];
      sprintf(errmsg, "SEMANTIC ERROR: Duplicate declaration of '%s'", $1);
      yyerror(errmsg);
    }
    else
    {
      symbol_add($1);
    }
  }
  |
  id_list COMMA IDENTIFIER
  { 
    // semantic check for duplicate declarations
    if (symbol_exists($3)) 
    {
      char errmsg[256];
      sprintf(errmsg, "SEMANTIC ERROR: Duplicate declaration of '%s'", $3);
      yyerror(errmsg);
    } 
    else 
    {
      symbol_add($3); // Add to memory
    }
  }
  ;



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
  { 
    // semantic check for undeclared variable
    if (!symbol_exists($1)) {
        char errmsg[256];
        sprintf(errmsg, "SEMANTIC ERROR: Undeclared variable '%s'", $1);
        yyerror(errmsg);
    }
    $$ = ast_assign($1, $3); 
  }
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
  |
  expr NEQ term       { $$ = ast_operation(NEQ, $1, $3); }
  |
  expr LEQ term       { $$ = ast_operation(LEQ, $1, $3); }
  |
  expr GEQ term       { $$ = ast_operation(GEQ, $1, $3); }
  ;

term:
  factor              { $$ = $1; }
  |
  term MUL factor     { $$ = ast_operation(MUL, $1, $3); }
  |
  term DIV factor     { $$ = ast_operation(DIV, $1, $3); }
  |
  term REM factor     { $$ = ast_operation(REM, $1, $3); }
  |
  term MOD factor     { $$ = ast_operation(MOD, $1, $3); }
  ;
  
factor:
  INT                 { $$ = ast_integer($1); }
  |
  IDENTIFIER
  { 
    // semantic check for undeclared variable
    if (!symbol_exists($1)) {
        char errmsg[256];
        sprintf(errmsg, "SEMANTIC ERROR: Undeclared variable '%s'", $1);
        yyerror(errmsg);
    }
    $$ = ast_variable($1); 
  }
  |
  GET_LINE            { $$ = ast_getline(); }
  |
  LPAREN expr RPAREN  { $$ = $2; }
  ;

%%

//funcao de erro 

void yyerror(const char* err) {
  printf("Line %d: %s - at '%s'\n", yyline, err, yytext );
}

