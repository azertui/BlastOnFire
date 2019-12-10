%{
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"
  void yyerror(char*);
  extern int yylex();
%}

%union {
    double val;
    char* name;
    struct ast* ast;
}

%type <ast> ligne
%type <ast> function

%token INTEGER DOUBLE INTEGER_T DOUBLE_T ID SPACE
%%
 
ligne:
    function '\n'         { printf("Chaine reconnue !\n");ast_print($1,0);return 0;}
  ;

function:
    type SPACE ID '(' ')' '{'  '}' { $$ = ast_new_main_fct(); }
    ;
type:
    INTEGER_T
    | DOUBLE_T
    ;

%%

int parseFile(FILE* f){
    FILE* yyin=f;
    int yyflex();
    return yyparse();
}
extern int yy_scan_string(char*);
int parseString(char *s) {
    printf("%lu:%s",strlen(s),s);
  yy_scan_string(s);
  int yylex();
  return yyparse();
}

int parse() {
  printf("Entrez une expression :\n");
  return yyparse();
}