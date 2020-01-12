%{
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"
  #include "symboles.h"
  #include "y.tab.h"
  #include "lex.h"
  #include <stdlib.h>
  void yyerror(ast* a,void* scanner,int print_symb, const char* msg){
    (void)a;
    fprintf(stderr,"%s\n##########\n",msg);
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
    struct array *arr;
    ast_type type;
}
%type <ast> code
%type <ast> function
%type <ast> appel
%type <ast> parametres_appel
%type <ast> instruction
%type <ast> parametres_function
%type <ast> parametre_function
%type <ast> operation
%type <type> affectation_op
%type <ast> body
%type <ast> boucle
%type <ast> condition
%type <ast> condition_suite
%type <ast> ligne
%type <ast> boolean
%type <ast> unary
%type <val> pre_type
%type <ast> declaration
%type <ast> for_boolean
%type <ast> for_declaration
%type <ast> for_unary
%type <arr> array
%type <arr> array_function
%left '+' '-'
%left '*' '/' '%'

%token <val>INTEGER
%token <fval> DOUBLE
%token INTEGER_T DOUBLE_T CONST DECR INCR
%token IF ELSE FOR WHILE AND OR
%token <name>ID 
%parse-param {ast* parsed_ast} {void * scanner} {int print_symb}
%start start
%%
 
start:
    code {printf("Syntaxe reconnue!\n"); if(analyse_ast($1,print_symb))return 1; *parsed_ast=*$1;free($1); return 0;}
;

code:
     declaration ';' code {$$=$1;$1->next=$3;}
    |function code        {$$=$1; $1->next=$2;}
    | %empty/*epsilon*/   {$$=NULL;}
  ;

function:
     INTEGER_T ID '(' parametres_function ')' '{' body '}' { $$ = $4; $$->fonction.interne = $7; free($$->fonction.id); $$->fonction.id = $2; $$->fonction.returnType =  AST_INT; }
    
    |DOUBLE_T ID '(' parametres_function ')' '{' body '}'  { $$ = $4; $$->fonction.interne = $7; free($$->fonction.id); $$->fonction.id = $2; $$->fonction.returnType =  AST_DOUBLE; }
;

parametres_function:
      parametre_function ',' parametres_function {$$ = $3; $$->fonction.nb_param++; $$->fonction.params = realloc($$->fonction.params, $$->fonction.nb_param * sizeof(ast*)); $$->fonction.params[$$->fonction.nb_param-1] = $1; }
     |parametre_function                         {$$ = ast_new_main_fct(NULL, NULL, NULL, 0); $$->fonction.nb_param = 1; $$->fonction.params = malloc(sizeof(ast*)); $$->fonction.params[0] = $1;}
     |/*empty*/                                  {$$ = ast_new_main_fct(NULL, NULL, NULL, 0);}
;

parametre_function:
      pre_type INTEGER_T ID array_function       {if($4)$$=ast_new_tab_int($3, NULL, 0, $4, $1);else $$=ast_new_id($3, NULL, 0, $1, 1); free($3); }
;

array_function:
      '[' ']' array_function {$$=malloc(sizeof(struct array)); $$->n_dim=5;$$->next=$3;}
      |/*epsilon*/           {$$ = NULL;}
;

appel:
     ID '(' parametres_appel ')' { $$ = $3; $$->appel.id = $1; }
;

parametres_appel:
    operation ',' parametres_appel { $$ = $3; $$->appel.nb_param++; $$->appel.params = realloc($$->appel.params, $$->appel.nb_param * sizeof(ast*)); $$->appel.params[$$->appel.nb_param-1] = $1; }
    | operation                    {$$ = ast_new_app(NULL, 1, malloc(sizeof(ast*))); $$->appel.params[0] = $1; }
    | /*epsilon*/                  {$$ = ast_new_app(NULL, 0, NULL);}
;

body:
    ligne body { if($1!=NULL)$$ = ast_link($1,$2);else $$ = $2;}
    | %empty/*epsilon*/{$$ = NULL;}
;

ligne:
     instruction {$$ = $1;}

    | boucle {$$ = $1;}

    | condition {$$ = $1;}

    | '\n' { $$ = NULL;}
    
    | ';' { $$ = NULL;}
;

array:
  '[' INTEGER ']' array {if($2<0){fprintf(stderr,"invalid array size\n");return 1;}
                         $$=malloc(sizeof(struct array)); $$->n_dim=$2;$$->next=$4;}
  | %empty {$$=NULL;}
;


declaration:
     pre_type INTEGER_T ID array  {if ($4==NULL)$$ = ast_new_id($3,NULL,1,$1,1);else{$$ = ast_new_tab_int($3,NULL,1,$4,$1);}free($3);}
    | pre_type INTEGER_T ID '=' operation {$$ = ast_new_id($3,$5,1,$1,1);free($3);}
    | pre_type DOUBLE_T ID array               {if ($4==NULL)$$ = ast_new_id($3,NULL,1,$1,0);else{$$ = ast_new_tab_int($3,NULL,1,$4,$1);}free($3);}
    | pre_type DOUBLE_T ID '=' operation  { $$ = ast_new_id($3,$5,1,$1,0); free($3);}
;

unary:
      INCR ID {$$=ast_new_id($2,ast_new_operation(AST_OP_INCR,ast_new_id($2,NULL,0,0,0),NULL),0,0,0);free($2);}
    | ID INCR {$$=ast_new_id($1,ast_new_operation(AST_OP_INCR,ast_new_id($1,NULL,0,0,0),NULL),0,0,0);free($1);}

    | DECR ID {$$=ast_new_id($2,ast_new_operation(AST_OP_DECR,ast_new_id($2,NULL,0,0,0),NULL),0,0,0);free($2);}
    | ID DECR {$$=ast_new_id($1,ast_new_operation(AST_OP_DECR,ast_new_id($1,NULL,0,0,0),NULL),0,0,0);free($1);}
;

instruction:
      declaration ';'{$$=$1;}
    | unary ';' {$$=$1;}
    | ID array '=' operation ';'            {if($2==NULL) $$ = ast_new_id($1,$4,0,0,0) ; else $$ = ast_new_tab_int($1,$4,0,$2,0);free($1);}
    | ID array affectation_op '=' operation ';'  {if($2==NULL) $$ = ast_new_id($1,ast_new_operation($3,ast_new_id($1,NULL,0,0,0),$5),0,0,0);else $$ = ast_new_tab_int($1,ast_new_operation($3,ast_new_tab_int($1,NULL,0,$2,0),$5),0,$2,0);free($1);}
    | appel ';' {$$=$1;}
;

pre_type:
  %empty {$$=0;}
  | CONST {$$=1;}
;

condition:                                              
      IF '(' boolean ')' instruction condition_suite                  { if($6!=NULL)$$ = ast_link(ast_new_comparateur($3,$5,AST_IF),$6);else $$ = ast_new_comparateur($3,$5,AST_IF);}
    
    | IF '(' boolean ')' instruction                        {$$ = ast_new_comparateur($3,$5,AST_IF);}    

    | IF '(' boolean ')' '{' body '}' condition_suite                { $$ = ast_new_comparateur($3,$6,AST_IF);}    

    | IF '(' boolean ')' '{' body '}'                { $$ = ast_new_comparateur($3,$6,AST_IF);}    

;

boolean:
    operation '=' '=' operation {$$=ast_new_condition($1,$4,"==");}
  | operation '!' '=' operation {$$=ast_new_condition($1,$4,"!=");}
  | operation '<' '=' operation {$$=ast_new_condition($1,$4,"<=");}
  | operation '>' '=' operation {$$=ast_new_condition($1,$4,">=");}
  | operation '<' operation {$$=ast_new_condition($1,$3,"<");}
  | operation '>' operation {$$=ast_new_condition($1,$3,">");}
  | operation AND boolean {$$=ast_new_condition($1,$3,"&&");}
  | operation OR boolean {$$=ast_new_condition($1,$3,"||");}
  | '!' operation {$$=ast_new_condition($2,NULL,"false");}
  | operation {$$=ast_new_condition($1,NULL,"true");}
;

condition_suite:
      ELSE '{' body '}' {$$ = ast_new_comparateur(NULL,$3,AST_ELSE); }

    | ELSE instruction { $$ = ast_new_comparateur(NULL,$2,AST_ELSE);}

    | ELSE condition  { ast* test = ast_new_comparateur($2->comparateur.cond,$2->comparateur.interne,AST_ELSE_IF);ast_link(test,$2->next);free($2);$$ = test;}

;

for_declaration:
   declaration {$$=$1;}
  | ID '=' operation                  {$$ = ast_new_id($1,$3,0,0,0);free($1);}
  |%empty      {$$=NULL;}
;

for_boolean:
   boolean     {$$=$1;}
  |%empty      {$$=NULL;}
;

for_unary:
  unary        {$$=$1;}
  |%empty      {$$=NULL;}
;

boucle:
   FOR '(' for_declaration ';' for_boolean ';' for_unary ')' '{' body '}' {$$=ast_new_boucle_for($3,$5,$7,$10);}
  |FOR '(' for_declaration ';' for_boolean ';' for_unary ')' instruction  {$$=ast_new_boucle_for($3,$5,$7,$9);}
  |WHILE '(' for_boolean ')' '{' body '}' {$$ = ast_new_boucle_while($3,$6);}
  |WHILE '(' for_boolean ')' instruction {$$ = ast_new_boucle_while($3,$5);}
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
  | ID array { if($2==NULL){ $$ = ast_new_id($1,NULL,0,0,0);}else{ $$ = ast_new_tab_int($1,NULL,0,$2,0); } free($1);}
;

%%

int parseFile(FILE* f, ast *result_ast, int print_ast, int print_tab,char* filename){
  yyscan_t scanner;
  yylex_init (&scanner);
  yyset_debug(5, scanner);
  yyset_in(f,scanner);
  ast* parsed_ast=malloc(sizeof(ast));
  int res= yyparse(parsed_ast,scanner,print_tab);
  if(print_ast)
    ast_print(parsed_ast,0);
  ast_to_code(parsed_ast,filename);
  if(result_ast!=NULL && !res){
    *result_ast=*parsed_ast;
    free(parsed_ast);
  }else{
    free_ast(parsed_ast);
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
  ast* parsed_ast= malloc(sizeof(ast));
  int res= yyparse(parsed_ast,scanner,0);
  if(result_ast!=NULL && res==0){
    *result_ast=*parsed_ast;
    free(parsed_ast);
  }else{
    free_ast(parsed_ast);
  }
  yy_delete_buffer(buf, scanner);
  yylex_destroy(scanner);
  return res;
}
//TODO: màj la fonction
int parse(ast* result_ast) {
  yyscan_t scanner;
  yylex_init (&scanner);
  printf("Entrez une expression :\n");
  ast* parsed_ast=malloc(sizeof(ast));
  int res= yyparse(parsed_ast,scanner,0);
  if(result_ast==NULL && res==0){
    *result_ast=*parsed_ast;
    free(parsed_ast);
  }else{
    free_ast(parsed_ast);
  }
  yylex_destroy(scanner);
  return res;
}
