%{
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"
  #include "symboles.h"
  void yyerror(char*);
  extern int yylex();
  extern FILE *yyin;
  extern int yyparse();
  symboles tab_S;
%}

%union {
    int val;
    char* name;
    struct ast* ast;
}

%type <ast> ligne
%type <ast> function
%type <ast> instruction
%type <ast> operation
%type <ast> body

%left '+'
%left '*'
%left '-'
%left '/'

%token <val>INTEGER 
%token DOUBLE INTEGER_T DOUBLE_T SPACE 
%token <name>ID 
%%
 
ligne:
    function         { printf("Chaine reconnue !\n");ast_print($1,0);free_ast($1);free_symboles(tab_S);return 0;}
    | '\n'                { printf("Chaine reconnue !\n");return 0;}
  ;

function:
    INTEGER_T SPACE ID '(' ')' '{' body '}' { $$ = ast_new_main_fct($7); free($3);}
    ;

body:
    instruction body { $$ = ast_link($1,$2);}
    | %empty {$$ = NULL;}
;

instruction:
     INTEGER_T SPACE ID';'                  { $$ = ast_new_id($3,NULL); 
                                              tab_S = add_symbole(tab_S,$3,0); free($3);}
    | INTEGER_T SPACE ID '=' operation ';'  { $$ = ast_new_id($3,$5); 
                                              tab_S = add_symbole(tab_S,$3,0);free($3);}
    | ID '=' operation ';'                  { if(getSymbole(tab_S,$1)==NULL){printf("ID (%s) non reconnu\n",$1);free($1);return 1;} 
                                              $$ = ast_new_id($1,$3);free($1);}
    | '\n' { $$ = NULL;}
;

operation:
    operation '+' operation { $$ = ast_new_operation(AST_OP_PLUS,$1,$3);}
  | operation '*' operation { $$ = ast_new_operation(AST_OP_MUL,$1,$3);}
  | operation '/' operation { $$ = ast_new_operation(AST_OP_DIV,$1,$3);}
  | operation '-' operation { $$ = ast_new_operation(AST_OP_MOINS,$1,$3);}
  | INTEGER   {$$ = ast_new_number($1);}
  | ID        { if(getSymbole(tab_S,$1)==NULL){printf("ID (%s) non reconnu\n",$1);free($1);return 1;}
                $$ = ast_new_id($1,NULL);free($1);}
;

%%
extern int yylex_destroy();

int parseFile(FILE* f){
    yyin=f;
    tab_S = new_table();
    int res= yyparse();
    yylex_destroy();
    return res;
}
extern int yy_scan_string(char*);
int parseString(char *s) {
    printf("%lu:%s",strlen(s),s);
  yy_scan_string(s);
  int yylex();
  int res= yyparse();
  yylex_destroy();
  return res;
}

int parse() {
  printf("Entrez une expression :\n");
  int res= yyparse();
  yylex_destroy();
  return res;
}