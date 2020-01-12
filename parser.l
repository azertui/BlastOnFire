%{
    #include <stdlib.h>
    #include "ast.h"
    #include "y.tab.h"
%}
%option yylineno
%option reentrant
%option bison-bridge
%option header-file = "lex.h"
INTEGER int
DOUBLE double
NUMBER [0-9]+
ID [a-zA-Z_][a-zA-Z0-9_]*
COMMENT1 "/*"(.|\n)*"*/"
COMMENT2 "//"(.)*"\n"
%%

{COMMENT1} {}
{COMMENT2} {}
{INTEGER} {return INTEGER_T;}
{DOUBLE} {return DOUBLE_T;}
{NUMBER}"."{NUMBER} {yylval->fval=atof(yytext);return DOUBLE;}
"++" {return INCR;}
"--" {return DECR;}
"&&" {return AND;}
"||" {return OR;}
"if" {return IF;}
"else" {return ELSE;}
"for" {return FOR;}
"while" {return WHILE;}
"const" {return CONST;}
"return" {return RETURN;}
{NUMBER} {yylval->val=atoi(yytext);return INTEGER;}
{ID} {yylval->name=strdup(yytext); return ID;}
[\!\%] {return yytext[0];}
[;,{(\[\])}=+*/\-<>] {return yytext[0];}
[[:blank:]\n] {}
%%
