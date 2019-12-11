    %{
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"
  void yyerror(char*);
  extern int yylex();
%}

%union {
    int val;
    char* name;
    struct ast* ast;
}

%type <ast> ligne
%type <ast> function
%type <ast> body

%token <val>INTEGER 
%token DOUBLE INTEGER_T DOUBLE_T SPACE 
%token <name>ID 
%%
 
ligne:
    function '\n'         { printf("Chaine reconnue !\n");ast_print($1,0);return 0;}
    | '\n'                { printf("Chaine reconnue !\n");return 0;}
  ;

function:
    INTEGER_T SPACE ID '(' ')' '{' body '}' { $$ = ast_new_main_fct($7); }
    ;

body:
    INTEGER_T SPACE ID ';' { $$ = ast_new_id($3,NULL);}
  | INTEGER_T SPACE ID '=' INTEGER ';' { $$ = ast_new_id($3,$5);}
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
