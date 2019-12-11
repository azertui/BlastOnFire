/**
 * \file ast.h
 * \brief Header de l'arbre de syntaxe
 * \author Monfouga Marie
 * \version 0.1
 * \date 11 décembre 2019
 *
 * Header de l'arbre de syntaxe
 *
 */


enum ast_type { AST_ID, AST_NUMBER, AST_OP_PLUS, AST_OP_MUL, AST_FCT};

typedef struct ast {
  enum ast_type type;
  union {
    struct {
      struct ast* left;
      struct ast* right; 
    } operation;
    struct {
      char* id;
      int number;
    } type_int;
    int number;
    char* id;
  } ;
  struct ast* next;
} ast;

ast* ast_new_main_fct(ast*);
ast* ast_new_operation(enum ast_type, ast*, ast*);
ast* ast_new_number(int);
ast* ast_new_id(char*,int);
void ast_print(ast*, int);
