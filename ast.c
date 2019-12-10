#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ast* ast_new_main_fct(){
  ast* new = malloc(sizeof(ast));
  new->type = AST_FTC;
  new->id = "main";
  return new; 
}

ast* ast_new_operation(enum ast_type type, ast* left, ast* right) {
  ast* new = malloc(sizeof(ast));
  new->type = type;
  new->operation.left = left;
  new->operation.right = right;
  return new;
}

ast* ast_new_number(int number) {
  ast* new = malloc(sizeof(ast));
  new->type = AST_NUMBER;
  new->number = number;
  return new;
}

ast* ast_new_id(char* id) {
  ast* new = malloc(sizeof(ast));
  new->type = AST_ID;
  new->id = strdup(id);
  return new;
}

void ast_print(ast* ast, int indent) {
  for (int i = 0; i < indent; i++)
    printf("    ");
  switch(ast->type){
    case AST_FCT:
      printf("FCT (%s)\n",ast->id);
      break;    
    case AST_ID:
      printf("ID (%s)\n",ast->id);
      break;
    case AST_NUMBER:
      printf("NUMBER (%d)\n",ast->number);
      break;
    case AST_OP_PLUS:
      printf("+\n");
      ast_print(ast->operation.left,indent+1);
      ast_print(ast->operation.right,indent+1);
      break;
    case AST_OP_MUL:
      printf("*\n");
      ast_print(ast->operation.left,indent+1);
      ast_print(ast->operation.right,indent+1);
      break;
  }  
}


 int main (){
   
   "int a;"
 }