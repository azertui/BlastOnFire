%{
  #include <stdio.h>
  #include <string.h>
  void yyerror(char*);
  extern int yylex();
%}

%union {
    double val;
    char* name;
}

%token INTEGER DOUBLE INTEGER_T DOUBLE_T ID SPACE
%%
 
ligne:
    function '\n'         { printf("Chaine reconnue !\n");return 0;}
  ;

function:
    type SPACE ID '(' liste ')' '{' body '}'
    |type SPACE ID '(' ')' '{' body '}'
    ;
type:
    INTEGER_T
    | DOUBLE_T
    ;
liste:
    type SPACE ID
    |type SPACE ID ',' liste
    ;
body:
    |type SPACE ID '=' value ';' body
    |type SPACE ID ';' body
    ;
value:
    INTEGER
    |DOUBLE
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
  int yylex();
  return yyparse();
}