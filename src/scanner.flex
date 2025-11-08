%{
// HEADERS
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "ast.h"

/*variables maintained by the lexical analyser*/
int yyline = 1;
%}

%option noyywrap
%option caseless

%%
[ \t]+ {  } /*skip whitespaces*/
"--".*\n { yyline++; } /*skip comments*/
\n { yyline++; }

\-?[0-9]+  /*ints*/  { 
   yylval.intValue = atoi(yytext);
   return INT; 
}

"procedure" { return PROCEDURE; }
"main"      { return MAIN; }
"is"        { return IS; }
"begin"     { return TOK_BEGIN; }
"end"       { return END; }
"integer"   { return INTEGER_TYPE; }
"if"        { return IF; }
"then"      { return THEN; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"loop"      { return LOOP; }
"put_line"  { return PUT_LINE; }
"get_line"  { return GET_LINE; }

":="  { return ASSIGN; }
";"   { return SEMICOLON; }
":"   { return COLON; }
","   { return COMMA; }
"("   { return LPAREN; }
")"   { return RPAREN; }

"+"   { return PLUS; }
"-"   { return MINUS; }
"*"   { return MUL; }
"/"   { return DIV; }
"<"   { return LT; }
">"   { return GT; }
"="   { return EQ; }
"/="  { return NEQ; }
"<="  { return LEQ; }
">="  { return GEQ; } 
"rem" { return REM; }
"mod" { return MOD; }


[a-zA-Z_][a-zA-Z0-9_]*  /*identifiers for variables*/  { 
    yylval.strValue = strdup(yytext); 
    return IDENTIFIER; 
}

.  { yyerror("unexpected character"); }
%%

