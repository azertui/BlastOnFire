%{
  #include <stdio.h>
  #include <string.h>

  #include "ast.h"

  void yyerror(ast**, char*);
  extern int yylex();
  extern FILE *yyin;
  extern int yyparse();
  extern int yy_scan_string(char*);

  int parseFile(FILE* f, ast** out);
  int parseString(char *s, ast** out);
  int parse(ast** out);
%}

%union {
    int val;
    char* name;
    struct ast* ast;
}

/* Paramètre de sortie du parser - l'AST*/
%parse-param {ast** out}

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
    function         { printf("Chaine reconnue !\n");ast_print($1,0);return 0;}
    | '\n'                { printf("Chaine reconnue !\n");return 0;}
  ;

function:
    INTEGER_T SPACE ID '(' ')' '{' body '}' { $$ = ast_new_main_fct($7); }
    ;

body:
    instruction body { $$ = ast_link($1,$2);}
    | %empty {$$ = NULL;}
;

instruction:
     INTEGER_T SPACE ID';'  { $$ = ast_new_id($3,NULL);}
    | INTEGER_T SPACE ID '=' operation ';'  { $$ = ast_new_id($3,$5);}
    | '\n' { $$ = NULL;}
;

operation:
    operation '+' operation { $$ = ast_new_operation(AST_OP_PLUS,$1,$3);}
  | operation '*' operation { $$ = ast_new_operation(AST_OP_MUL,$1,$3);}
  | operation '/' operation { $$ = ast_new_operation(AST_OP_DIV,$1,$3);}
  | operation '-' operation { $$ = ast_new_operation(AST_OP_MOINS,$1,$3);}
  | INTEGER   {$$ = ast_new_number($1);}
;

%%

int parseFile(FILE* f, ast** out){
    yyin=f;
    return yyparse(out);
}

int parseString(char *s, ast** out) {
    printf("%lu:%s",strlen(s),s);
  yy_scan_string(s);
  int yylex();
  return yyparse(out);
}

int parse(ast** out) {
  printf("Entrez une expression :\n");
  return yyparse(out);
}
