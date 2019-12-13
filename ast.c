/**
 * \file ast.c
 * \brief Source de l'arbre de syntaxe
 * \author Monfouga Marie
 * \version 0.1
 * \date 11 décembre 2019
 *
 * Code source des fonctions de l'arbre de syntaxe
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ast* ast_new_main_fct(ast* next){
  ast* new = malloc(sizeof(ast));
  new->type = AST_FCT;
  new->id = "main";
  new->next=next;
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

ast* ast_new_id(char* id, ast* value) {
  ast* new = malloc(sizeof(ast));
  new->type = AST_ID;
  new->type_int.id = strndup(id,strlen(id));
  new->type_int.value = value;
  return new;
}

void ast_print(ast* ast, int indent) {
  if (ast!=NULL){  
  for (int i = 0; i < indent; i++)
    printf("    ");    
    switch(ast->type){
      case AST_FCT:
        printf("FCT (%s)\n",ast->id);
        ast_print(ast->next,indent);
        break;    
      case AST_ID:
        printf("ID (%s)",ast->type_int.id);
        if (ast->type_int.value!=NULL){
          printf(" = \n");ast_print(ast->type_int.value,indent+1);
        }
        else
          printf("\n");
        
        ast_print(ast->next,indent);
        break;
      case AST_NUMBER:
        printf("NUMBER (%d)\n",ast->number);
        break;
      case AST_OP_PLUS:
        printf("+\n");
        ast_print(ast->operation.left,indent+1);
        ast_print(ast->operation.right,indent+1);
        ast_print(ast->next,indent);
        break;
      case AST_OP_MUL:
        printf("*\n");
        ast_print(ast->operation.left,indent+1);
        ast_print(ast->operation.right,indent+1);
        ast_print(ast->next,indent);
        break;
      case AST_OP_MOINS:
        printf("-\n");
        ast_print(ast->operation.left,indent+1);
        ast_print(ast->operation.right,indent+1);
        ast_print(ast->next,indent);
        break;
      case AST_OP_DIV:
        printf("/\n");
        ast_print(ast->operation.left,indent+1);
        ast_print(ast->operation.right,indent+1);
        ast_print(ast->next,indent);
        break;      
    }  
  }
}

ast* ast_link(ast* a, ast* next){
  a->next = next;
  return a;
}


  /*int main(){
    ast* cinq = ast_new_number(5);
    ast* deux = ast_new_number(2); 
    ast* dix = ast_new_number(10); 
    ast* test = ast_new_id("a",cinq);
    ast* test2 = ast_new_id("b",deux);
    ast* test3 = ast_new_operation(AST_OP_MOINS,cinq,ast_new_id("b",deux));
    ast* test4 = ast_new_id("c",dix);
    test3 = ast_link(test3,test4);
    test2 = ast_link(test2,test3);
    test = ast_link(test,test2);
    test = ast_new_main_fct(test);
    ast_print(test,0);
  }*/