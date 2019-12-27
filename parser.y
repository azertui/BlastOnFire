%{
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"
  #include "symboles.h"
  #include "y.tab.h"
  #include "lex.h"
  void yyerror(ast*,void*,char*);
  symboles tab_S;
%}
%debug
%lex-param {void * scanner}
%pure-parser
%union {
    int val;
    char* name;
    struct ast* ast;
    ast_type type;
}

%type <ast> code
%type <ast> function
%type <ast> instruction
%type <ast> operation
%type <type> affectation_op
%type <ast> body
%type <ast> condition
%type <ast> condition_suite
%type <ast> ligne

%left '+'
%left '*'
%left '-'
%left '/'

%token <val>INTEGER 
%token DOUBLE INTEGER_T DOUBLE_T 
%token IF ELSE FOR WHILE
%token <name>ID 
%parse-param {ast* parsed_ast} {void * scanner}
%%
 
code:
    function         { printf("Chaine reconnue !\n");*parsed_ast=*$1;ast_print($1,0);ast_to_code($1);free_ast($1);free_symboles(tab_S);return 0;}
    | '\n'                { printf("Chaine reconnue !\n");return 0;}
  ;

function:
    INTEGER_T ID '(' ')' '{' body '}' { $$ = ast_new_main_fct($6); free($2);}
    ;

body:
    ligne body { if($1!=NULL)$$ = ast_link($1,$2);else $$ = $2;}
    | /*epsilon*/{$$ = NULL;}
;

ligne:
     instruction {$$ = $1;}

    | condition {$$ = $1;}

    | '\n' { $$ = NULL;}
    
    | ';' { $$ = NULL;}
;

instruction:
     INTEGER_T ID';'                  { $$ = ast_new_id($2,NULL,1); 
                                              tab_S = add_symbole(tab_S,$2,0); free($2);}
    | INTEGER_T ID '=' operation ';'  { $$ = ast_new_id($2,$4,1); 
                                              tab_S = add_symbole(tab_S,$2,0);free($2);}
    | ID '=' operation ';'                  { if(getSymbole(tab_S,$1)==NULL){printf("ID (%s) non reconnu\n",$1);free($1);return 1;} 
                                              $$ = ast_new_id($1,$3,0);free($1);}
    | ID affectation_op '=' operation              { if(getSymbole(tab_S,$1)==NULL){printf("ID (%s) non reconnu\n",$1);free($1);return 1;} 
                                                        $$ = ast_new_id($1,ast_new_operation($2,ast_new_id($1,NULL,0),$4),0);free($1);}
;

condition:                                              
     IF '(' operation operation ')' instruction               { $$ = ast_new_condition($3,$4,"==",$6,AST_IF); }    

    | IF '(' operation operation ')' instruction condition_suite                  { if($7!=NULL)$$ = ast_link(ast_new_condition($3,$4,"==",$6,AST_IF),$7);else $$ = ast_new_condition($3,$4,"==",$6,AST_IF); }    

    | IF '(' operation operation ')' '\n' instruction condition_suite                  { if($8!=NULL)$$ = ast_link(ast_new_condition($3,$4,"==",$7,AST_IF),$8);else $$ = ast_new_condition($3,$4,"==",$7,AST_IF); }    

    | IF '(' operation operation ')' '{' body '}' condition_suite                { $$ = ast_new_condition($3,$4,"==",$7,AST_IF); }    
                                                                    
;

condition_suite:
      ELSE '{' body '}' { $$ = ast_new_condition(NULL,NULL,NULL,$3,AST_ELSE); }

    | ELSE instruction  { $$ = ast_new_condition(NULL,NULL,NULL,$2,AST_ELSE); }

    | ELSE condition  { $$ = ast_new_condition($2->condition.left,$2->condition.right,$2->condition.op,$2->condition.interne,AST_ELSE_IF);}

;

affectation_op:
      '+'             { $$ = AST_OP_PLUS;}
    | '-'             { $$ = AST_OP_MOINS;}
    | '*'             { $$ = AST_OP_MUL;}
    | '/'             { $$ = AST_OP_DIV;}
;

operation:
    '(' operation ')' {$$ = $2;}
  | operation '+' operation { $$ = ast_new_operation(AST_OP_PLUS,$1,$3);}
  | operation '*' operation { $$ = ast_new_operation(AST_OP_MUL,$1,$3);}
  | operation '/' operation { $$ = ast_new_operation(AST_OP_DIV,$1,$3);}
  | operation '-' operation { $$ = ast_new_operation(AST_OP_MOINS,$1,$3);}
  | INTEGER   {$$ = ast_new_number($1);}
  | ID        { if(getSymbole(tab_S,$1)==NULL){printf("ID (%s) non reconnu\n",$1);free($1);return 1;}
                $$ = ast_new_id($1,NULL,0);free($1);}
;

%%

int parseFile(FILE* f, ast *result_ast){
  yyscan_t scanner;
  yylex_init (&scanner);
  yyset_in(f,scanner);
  tab_S = new_table();
  ast parsed_ast;
  int res= yyparse(&parsed_ast,scanner);
  if(result_ast!=NULL){
    *result_ast=parsed_ast;
  }
  yylex_destroy(scanner);
  return res;
}

int parseString(char *s,ast *result_ast ) {
  yyscan_t scanner;
  tab_S = new_table();
  if(yylex_init (&scanner)){
    perror("parseString");
    return 1;
  }
  YY_BUFFER_STATE buf =yy_scan_string(s,scanner);
  ast parsed_ast;
  int res= yyparse(&parsed_ast,scanner);
  if(result_ast!=NULL){
    *result_ast=parsed_ast;
  }
  yy_delete_buffer(buf, scanner);
  yylex_destroy(scanner);
  return res;
}

int parse() {
  tab_S = new_table();
  yyscan_t scanner;
  yylex_init (&scanner);
  printf("Entrez une expression :\n");
  ast parsed_ast;
  int res= yyparse(&parsed_ast,scanner);
  yylex_destroy(scanner);
  return res;
}