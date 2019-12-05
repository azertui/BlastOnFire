%{
  #include <stdio.h>
  #include <stdlib.h>
  int yylex();
  void yyerror(char*);
%}

%union {
	int		val;
}

%token INTEGER TYPE_INT SPACE
%type <val> value
%%

ligne:
  declaration '\n' {printf("Chaine reconnue !\n"); }
  ;

declaration:
  TYPE_INT escape value escape ';' {printf("int %d;\n",$3);}
  ;

value:
  INTEGER {$$=yylval.val;}
  ;
escape:
  | SPACE escape
  ;
%%

int main() {
  printf("Entrez une expression :\n");
  return yyparse();
}
