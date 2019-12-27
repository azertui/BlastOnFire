%{
    #include <stdlib.h>
    #include "ast.h"
    #include "y.tab.h"
%}
%option reentrant
%option bison-bridge
%option header-file = "lex.h"
INTEGER int
DOUBLE double
NUMBER [0-9]+
ID [a-zA-Z]+


%%
{INTEGER} {return INTEGER_T;}
{DOUBLE} {return DOUBLE_T;}
{NUMBER}\.{NUMBER} {yylval->val=atof(yytext);return DOUBLE;}
"&&" {return AND;}
"||" {return OR;}
"if" {return IF;}
"else" {return ELSE;}
"for" {return FOR;}
"while" {return WHILE;}
{NUMBER} {yylval->val=atoi(yytext);return INTEGER;}
{ID} {yylval->name=strdup(yytext); return ID;}
[\!\%] {return yytext[0];}
[\n;,{()}=+*\-] {return yytext[0];}
[[:blank:]] {}
%%