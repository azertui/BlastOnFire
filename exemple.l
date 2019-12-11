%{
    #include <stdlib.h>
    #include "y.tab.h"
%}

INTEGER int
DOUBLE double
NUMBER [0-9]+
ID [a-zA-Z]+
INSTRUCTIONS_TAG 'INSTRUCTIONS'
%%
{INTEGER} {return INTEGER_T;}
{DOUBLE} {return DOUBLE_T;}
{INSTRUCTIONS_TAG}
{NUMBER}\.{NUMBER} {yylval.val=atof(yytext);return DOUBLE;}
{NUMBER} {yylval.val=atoi(yytext);return INTEGER;}
{ID} {yylval.name=strdup(yytext); return ID;}
[ ]+ {return SPACE;}
[\n;,{()}=] {return yytext[0];}
%%
