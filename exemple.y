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
%type <ast> instructions
%type <ast> type_int
%type <ast> operation

%left '+'
%left '*'
%left '-'
%left '/'

%token <val>INTEGER 
%token DOUBLE INTEGER_T DOUBLE_T SPACE 
%token <name>ID 
%%
 
ligne:
    function '\n'         { printf("Chaine reconnue !\n");ast_print($1,0);return 0;}
    | '\n'                { printf("Chaine reconnue !\n");return 0;}
  ;

function:
    INTEGER_T SPACE ID '(' ')' '{''}' {$$ = ast_new_main_fct(NULL);}
    | INTEGER_T SPACE ID '(' ')' '{' instructions '}' { $$ = ast_new_main_fct($7); }
    | instructions '\n'
    ;

instructions:
    instructions type_int { $$ = ast_link($1,$2);}
    | INTEGER_T SPACE ID type_int ';'  { $$ = ast_new_id($3,$4);}
    | '\n' { $$ = NULL;}
;

type_int:
     '' { $$ = NULL;}
  |  '=' operation  {$$ = $2}
  ;

operation:
    operation '+' operation { $$ = ast_new_operation(AST_OP_PLUS,$1,$3);}
  | operation '*' operation { $$ = ast_new_operation(AST_OP_MULT,$1,$3);}
  | operation '/' operation { $$ = ast_new_operation(AST_OP_DIV,$1,$3);}
  | operation '-' operation { $$ = ast_new_operation(AST_OP_MOINS,$1,$3);}
  | ID       {}
  | NUMBER   {$$ = ast_new_number($1);}
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