/**
 * \file ast.c
 * \brief Source de l'arbre de syntaxe
 * \author Monfouga Marie
 * \version 0.2
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

ast* ast_new_id(char* id, ast* value, int init) {
  ast* new = malloc(sizeof(ast));
  new->type = AST_ID;
  new->type_int.id = strndup(id,strlen(id));
  new->type_int.value = value;
  new->type_int.init = init;
  return new;
}

ast* ast_new_condition(ast* left, ast* right, char* op){
  ast* new = malloc(sizeof(ast));
  new->type = AST_IF;
  new->condition.left = left;
  new->condition.right = right;
  new->condition.op = strndup(op,strlen(op));
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
      case AST_IF:
        printf("IF : %s",ast->condition.op);
        ast_print(ast->condition.left,indent+1);
        ast_print(ast->condition.right,indent+1);
        ast_print(ast->condition.interne,indent+2);
        ast_print(ast->next,indent);
        break;   
    }  
  }
}

ast* ast_link(ast* a, ast* next){
  a->next = next;
  return a;
}

void free_ast(ast* ast){
  if (ast!=NULL){  
  switch(ast->type){
    case AST_FCT:
      free_ast(ast->next);
      free(ast);
      break;    
    case AST_ID:
      free_ast(ast->next);
      free_ast(ast->type_int.value);
      free(ast->type_int.id);
      free(ast);
      break;
    case AST_NUMBER:
      free(ast);
      break;
    case AST_OP_PLUS:
    case AST_OP_MUL:
    case AST_OP_MOINS:
    case AST_OP_DIV:
      free_ast(ast->next);
      free_ast(ast->operation.left);
      free_ast(ast->operation.right);
      free(ast);
      break;
    case AST_IF:
      free(ast->condition.op);
      free_ast(ast->condition.left);
      free_ast(ast->condition.right);
      free_ast(ast->condition.interne);
      free_ast(ast->next);
      free(ast);
      break;      
  }  
  }
}

void ast_to_code(ast* a){
  ast* parcours = a;
  FILE* fichier = NULL;
  fichier = fopen("res_c.c", "w");
  if (fichier != NULL)
    ast_to_code_recur(parcours,fichier);
    fclose(fichier);     
}

void ast_to_code_recur(ast* a, FILE* fichier){
      if (a!=NULL){
        switch(a->type){
          case AST_FCT:
            fputs("int main(){\n", fichier);
            ast_to_code_recur(a->next,fichier);
            fputs(";\nreturn 0;\n}", fichier);
            break; 
          case AST_ID:
            if (a->type_int.init)
              fputs("int ", fichier);
            fprintf(fichier, "%s", a->type_int.id);
            if(a->type_int.value!=NULL){
              fputs(" = ", fichier);
              ast_to_code_recur(a->type_int.value,fichier);
            }
            if (a->next!=NULL)
              fputs(";\n", fichier);
            ast_to_code_recur(a->next,fichier);
            break;
          case AST_NUMBER:
            fprintf(fichier, "%d", a->number);
            break;
          case AST_OP_PLUS:
            ast_to_code_recur(a->operation.left,fichier);
            fputs(" + ",fichier);
            ast_to_code_recur(a->operation.right,fichier);
            break;
          case AST_OP_MOINS:
            ast_to_code_recur(a->operation.left,fichier);
            fputs(" - ",fichier);
            ast_to_code_recur(a->operation.right,fichier);
            break;
          case AST_OP_MUL:
            ast_to_code_recur(a->operation.left,fichier);
            fputs(" * ",fichier);
            ast_to_code_recur(a->operation.right,fichier);
            break; 
          case AST_OP_DIV:
            ast_to_code_recur(a->operation.left,fichier);
            fputs(" / ",fichier);
            ast_to_code_recur(a->operation.right,fichier);
            break;    
          case AST_IF:
            fputs("if (",fichier);          
            ast_to_code_recur(a->condition.left,fichier);
            fprintf(fichier, "%s", a->condition.op);
            ast_to_code_recur(a->condition.right,fichier);
            fputs("){\n",fichier);
            ast_to_code_recur(a->condition.interne,fichier);
            fputs("}\n",fichier);        
            ast_to_code_recur(a->next,fichier);
            break;   
        }
    }
}

  int main(){
    ast* cinq = ast_new_number(5);
    ast* deux = ast_new_number(2); 
    ast* dix = ast_new_number(10); 
    ast* test = ast_new_id("a",cinq,1);
    ast* test2 = ast_new_id("b",deux,1);
    ast* test3 = ast_new_operation(AST_OP_MOINS,cinq,ast_new_id("b",NULL,0));
    ast* test4 = ast_new_id("c",dix,1);
    ast* test5 = ast_new_condition(ast_new_id("b",NULL,0),ast_new_number(5),"==");
    test4 = ast_link(test4,test5);
    test2 = ast_link(test2,test4);
    test = ast_link(test,test2);
    test = ast_new_main_fct(test);
    ast_print(test,0);
    ast_to_code(test);
    free_ast(test);
  }