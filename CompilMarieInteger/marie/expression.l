%{
  #include <stdlib.h>
  #include "y.tab.h"
%}

%%

int        { return TYPE_INT;}   
[0-9]+				{ yylval.val = atoi(yytext); return INTEGER; }
[\n]         { return yytext[0]; }
[ ] {return SPACE;}
[;]         { return yytext[0]; }
.            { printf("Caractere inconnu: %s", yytext); }

%%
