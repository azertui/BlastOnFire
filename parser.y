%{
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"
  #include "symboles.h"
  #include "y.tab.h"
  #include "lex.h"
  void yyerror(ast* a,void* scanner,const char* msg){
    fprintf(stderr,"%s\n##########\n",msg);
    printf("contenu ast:\n");
    ast_print(a,0);
    fprintf(stderr,"lineno:%d column:%d content:%s\n##########\n",yyget_lineno(scanner),yyget_column(scanner),yyget_text(scanner));
  };
%}
%debug
%lex-param {void * scanner}
%pure-parser
%union {
    int val;
    double fval;
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
%type <ast> boolean
%type <val> pre_type
%type <ast> declaration

%left '+' '-'
%left '*' '/' '%'

%token <val>INTEGER
%token <fval> DOUBLE
%token INTEGER_T DOUBLE_T CONST DECR INCR
%token IF ELSE FOR WHILE AND OR
%token <name>ID 
%parse-param {ast* parsed_ast} {void * scanner}
%start start
%%
 
start:
    code {printf("Chaine reconnue!\n"); *parsed_ast=*$1; ast_print($1,0); if(analyse_ast($1))return 1; ast_to_code($1); free_ast($1); return 0;}
;

code:
     declaration ';' code {$$=$1;$1->next=$3;}
    |function code        {$$=$1; $1->next=$2;}
    | %empty/*epsilon*/ {$$=NULL;}
  ;

function:
     INTEGER_T ID '(' ')' '{' body '}' { $$ = ast_new_main_fct($6,NULL,$2,AST_INT); free($2);}
    
    |DOUBLE_T ID '(' ')' '{' body '}' { $$ = ast_new_main_fct($6,NULL,$2,AST_DOUBLE); free($2);}
;

body:
    ligne body { if($1!=NULL)$$ = ast_link($1,$2);else $$ = $2;}
    | %empty/*epsilon*/{$$ = NULL;}
;

ligne:
     instruction {$$ = $1;}

    | condition {$$ = $1;}

    | '\n' { $$ = NULL;}
    
    | ';' { $$ = NULL;}
;

declaration:
     pre_type INTEGER_T ID                 { $$ = ast_new_id($3,NULL,1,$1); free($3);}
    | pre_type INTEGER_T ID '=' operation   { $$ = ast_new_id($3,$5,1,$1); free($3);}
    | pre_type DOUBLE_T ID                { $$ = ast_new_id($3,NULL,1,$1); free($3);}
    | pre_type DOUBLE_T ID '=' operation  { $$ = ast_new_id($3,$5,1,$1); free($3);}
;

instruction:
      declaration ';'{$$=$1;}

    | INCR  ID {$$=ast_new_id($2,ast_new_operation(AST_OP_INCR,ast_new_id($2,NULL,0,0),NULL),0,0);free($2);}
    | ID INCR {$$=ast_new_id($1,ast_new_operation(AST_OP_INCR,ast_new_id($1,NULL,0,0),NULL),0,0);free($1);}

    | DECR ID {$$=ast_new_id($2,ast_new_operation(AST_OP_DECR,ast_new_id($2,NULL,0,0),NULL),0,0);free($2);}
    | ID DECR {$$=ast_new_id($1,ast_new_operation(AST_OP_DECR,ast_new_id($1,NULL,0,0),NULL),0,0);free($1);}

    | ID '=' operation ';'                  {$$ = ast_new_id($1,$3,0,0);free($1);}
    | ID affectation_op '=' operation       {$$ = ast_new_id($1,ast_new_operation($2,ast_new_id($1,NULL,0,0),$4),0,0);free($1);}
;

pre_type:
  %empty {$$=0;}
  | CONST {$$=1;}
;

condition:                                              
      IF '(' boolean ')' instruction condition_suite                  { if($6!=NULL)$$ = ast_link(ast_new_condition($3->condition.left,$3->condition.right,$3->condition.op,$5,AST_IF),$6);else $$ = ast_new_condition($3->condition.left,$3->condition.right,$3->condition.op,$5,AST_IF); free($3->condition.op);free($3);}
    
    | IF '(' boolean ')' instruction                        {$$ = ast_new_condition($3->condition.left,$3->condition.right,$3->condition.op,$5,AST_IF); free($3->condition.op);free($3);}    

    | IF '(' boolean ')' '{' body '}' condition_suite                { $$ = ast_new_condition($3->condition.left,$3->condition.right,$3->condition.op,$6,AST_IF); free($3->condition.op);free($3);}    
                                                                    
;

boolean:
    operation '=' '=' operation {$$=ast_new_condition($1,$4,"==",NULL,AST_IF);}
  | operation '!' '=' operation {$$=ast_new_condition($1,$4,"!=",NULL,AST_IF);}
  | operation '<' '=' operation {$$=ast_new_condition($1,$4,"<=",NULL,AST_IF);}
  | operation '>' '=' operation {$$=ast_new_condition($1,$4,">=",NULL,AST_IF);}
  | operation '<' operation {$$=ast_new_condition($1,$3,"<",NULL,AST_IF);}
  | operation '>' operation {$$=ast_new_condition($1,$3,">",NULL,AST_IF);}
  | operation AND operation {$$=ast_new_condition($1,$3,"&&",NULL,AST_IF);}
  | operation OR operation {$$=ast_new_condition($1,$3,"||",NULL,AST_IF);}
  | '!' operation {$$=ast_new_condition($2,NULL,"false",NULL,AST_IF);}
  | operation {$$=ast_new_condition($1,NULL,"true",NULL,AST_IF);}
;

condition_suite:
      ELSE '{' body '}' { $$ = ast_new_condition(NULL,NULL,NULL,$3,AST_ELSE);}

    | ELSE instruction { $$ = ast_new_condition(NULL,NULL,NULL,$2,AST_ELSE);}

    | ELSE condition { $$ = ast_new_condition($2->condition.left,$2->condition.right,$2->condition.op,$2->condition.interne,AST_ELSE_IF);free($2->condition.op);free($2);}

;

affectation_op:
      '+'             { $$ = AST_OP_PLUS;}
    | '-'             { $$ = AST_OP_MOINS;}
    | '*'             { $$ = AST_OP_MUL;}
    | '/'             { $$ = AST_OP_DIV;}
    | '%'             { $$= AST_OP_MODULO;}
;

operation:
    '(' operation ')' {$$ = $2;}
  | operation '+' operation { $$ = ast_new_operation(AST_OP_PLUS,$1,$3);}
  | operation '*' operation { $$ = ast_new_operation(AST_OP_MUL,$1,$3);}
  | operation '/' operation { $$ = ast_new_operation(AST_OP_DIV,$1,$3);}
  | operation '-' operation { $$ = ast_new_operation(AST_OP_MOINS,$1,$3);}
  | operation '%' operation { $$ = ast_new_operation(AST_OP_MODULO,$1,$3);}
  | INTEGER   {$$ = ast_new_number($1,1);}
  | DOUBLE {$$=ast_new_number($1,0);}
  | ID        {$$ = ast_new_id($1,NULL,0,0);free($1);}
;

%%

int parseFile(FILE* f, ast *result_ast){
  yyscan_t scanner;
  yylex_init (&scanner);
  yyset_debug(5, scanner);
  yyset_in(f,scanner);
  ast parsed_ast;
  int res= yyparse(&parsed_ast,scanner);
  if(res) return res;
  if(result_ast!=NULL){
    *result_ast=parsed_ast;
  }
  yylex_destroy(scanner);
  return res;
}

int parseString(char *s,ast *result_ast ) {
  yyscan_t scanner;
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
  yyscan_t scanner;
  yylex_init (&scanner);
  printf("Entrez une expression :\n");
  ast parsed_ast;
  int res= yyparse(&parsed_ast,scanner);
  yylex_destroy(scanner);
  return res;
}