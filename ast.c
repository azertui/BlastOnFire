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

#define YYERROR_VERBOSE 1

ast* ast_new_main_fct(ast* body,ast* next,char* id,ast_type returnType){
  ast* new = malloc(sizeof(ast));
  new->type = AST_FCT;
  attribute_uid(new);
  new->fonction.id = strndup(id,strlen(id));
  new->fonction.interne=body;
  new->fonction.returnType=returnType;
  new->next=next;
  return new;
}

ast* ast_new_operation(ast_type type, ast* left, ast* right) {
  ast* new = malloc(sizeof(ast));
  new->type = type;
  new->operation.left = left;
  new->operation.right = right;
  new->next=NULL;
  return new;
}

ast* ast_new_number(double number,int is_int) {
  ast* new = malloc(sizeof(ast));
  new->type = AST_DOUBLE;
  if(is_int)
    new->type = AST_INT;
  new->number = number;
  new->next=NULL;
  return new;
}

ast* ast_new_id(char* id, ast* value, int init, int constant) {
  ast* new = malloc(sizeof(ast));
  new->type = AST_ID;
  new->type_int.id = strndup(id,strlen(id));
  new->type_int.value = value;
  new->type_int.init = init;
  new->type_int.constant=constant;
  new->next=NULL;
  return new;
}

void attribute_uid(ast* a){
  static unsigned int count=1;
  a->uid=count++;
}

char* ast_type_to_string1(ast_type t){
  char* tab[(AST_OP_DECR-AST_ID)+1]={"AST_ID", "int","double", "AST_OP_PLUS", "AST_OP_MUL", "AST_OP_MODULO", "AST_OP_MOINS", "AST_OP_DIV", "AST_FCT", "AST_IF", "AST_ELSE_IF" ,"AST_ELSE", "AST_OP_INCR","AST_OP_DECR"};
  return tab[t];
}

ast* ast_double_to_integer(ast* number){
  if(number->type==AST_INT){
    return number;
  } 
  if(number->type!=AST_DOUBLE){
    fprintf(stderr,"ast_double_to_integer: not an integer type\n");
    return NULL;
  }
  number->number=(int)number->number;
  number->type=AST_INT;
  return number;
}

ast* ast_new_condition(ast* left, ast* right, char* op){
  ast* new = malloc(sizeof(ast));
  new->type = AST_COND;
    new->condition.left = left;
    new->condition.right = right;
    if(op!=NULL)
      new->condition.op = strndup(op,strlen(op));
  new->next=NULL;
  return new;
}

ast* ast_new_comparateur(ast* cond,ast* interne, ast_type type){
  ast* new = malloc(sizeof(ast));
  new->type = type;
  attribute_uid(new);
  if (type == AST_IF || type == AST_ELSE_IF ){
    new->comparateur.cond = cond;
  }
  new->comparateur.interne = interne;
  new->next=NULL;
  return new;
}

ast* ast_new_boucle_for(ast* first,ast* second,ast* third,ast* interne){
  ast* new = malloc(sizeof(ast));
  new->type = AST_FOR;  
  attribute_uid(new);
  new->boucle_for.first = first;
  new->boucle_for.second = second;
  new->boucle_for.third = third;
  new->boucle_for.interne = interne;
  new->next=NULL;
  return new;
}

void ast_print(ast* ast, int indent) {
  if (ast!=NULL){  
    for (int i = 0; i < indent; i++)
      printf("    ");    
    switch(ast->type){
      case AST_FCT:
        printf("FCT (%s) type:%s\n",ast->fonction.id,ast_type_to_string1(ast->fonction.returnType));
        ast_print(ast->fonction.interne,indent+1);
        ast_print(ast->next,indent);
        break;    
      case AST_ID:
        printf("ID (%s)%s",ast->type_int.id,ast->type_int.constant?": const":"");
        if (ast->type_int.value!=NULL){
          printf(" = \n");ast_print(ast->type_int.value,indent+1);
        }
        else
          printf("\n");
        
        ast_print(ast->next,indent);
        break;
      case AST_DOUBLE:
        printf("NUMBER %s (%lf)\n",ast_type_to_string1(ast->type), ast->number);
        break;
      case AST_INT:
        printf("NUMBER %s (%d)\n",ast_type_to_string1(ast->type), (int) ast->number);
        break;
      case AST_OP_PLUS:
        printf("+ \n");
        ast_print(ast->operation.left,indent+1);
        ast_print(ast->operation.right,indent+1);
        ast_print(ast->next,indent);
        break;
      case AST_OP_MUL:
        printf("* \n");
        ast_print(ast->operation.left,indent+1);
        ast_print(ast->operation.right,indent+1);
        ast_print(ast->next,indent);
        break;
      case AST_OP_MOINS:
        printf("- \n");
        ast_print(ast->operation.left,indent+1);
        ast_print(ast->operation.right,indent+1);
        ast_print(ast->next,indent);
        break;
      case AST_OP_DIV:
        printf("/ \n");
        ast_print(ast->operation.left,indent+1);
        ast_print(ast->operation.right,indent+1);
        ast_print(ast->next,indent);
        break;   
      case AST_COND:
        if(ast->condition.op!=NULL)
          printf("%s",ast->condition.op);
        printf("\n");
        ast_print(ast->condition.left,indent+1);
        ast_print(ast->condition.right,indent+1);
        break;
      case AST_ELSE_IF:
        printf("ELSE " );  
      case AST_IF:
        printf("IF : \n");
        ast_print(ast->comparateur.cond,indent+1);
        ast_print(ast->comparateur.interne,indent+1);
        ast_print(ast->next,indent);
        break;
      case AST_ELSE:
        printf("ELSE : \n");
        ast_print(ast->comparateur.interne,indent+1);
        ast_print(ast->next,indent);   
        break;
      case  AST_OP_INCR:
        printf("++\n");
        break;
      case AST_OP_DECR:
        printf("--\n");
        break;
      case AST_FOR:
        printf("FOR (");
        ast_print(ast->boucle_for.first,indent+1);
        printf(";");
        ast_print(ast->boucle_for.second,indent+1);
        printf(";");
        ast_print(ast->boucle_for.third,indent+1);
        printf(")\n");
        ast_print(ast->boucle_for.interne,indent);
        break;
      default: fprintf(stderr,"print_ast:Unknown ast type:%d\n",ast->type);
    }  
  } 
}

ast* ast_link(ast* a, ast* next){
  ast* test = a;
  while (test->next!=NULL){
    test = test->next;
  }
  test->next = next;
  return a;
}

void free_ast(ast* a){
  if (a!=NULL){
  switch(a->type){
    case AST_FCT:
      free(a->fonction.id);
      free_ast(a->fonction.interne);
      free_ast(a->next);
      free(a);
      break;    
    case AST_ID:
      free_ast(a->type_int.value);
      free(a->type_int.id);
      free_ast(a->next);
      free(a);
      break;
    case AST_OP_PLUS:
    case AST_OP_MUL:
    case AST_OP_MOINS:
    case AST_OP_DIV:
      free_ast(a->operation.left);
      free_ast(a->operation.right);
      free_ast(a->next);
      free(a);
      break;
    case AST_COND:
      if(a->condition.op!=NULL)
        free(a->condition.op);
      free_ast(a->condition.left);
      free_ast(a->condition.right);
      free_ast(a->next);
      free(a);
      break;
    case AST_IF:
    case AST_ELSE_IF:
      free_ast(a->comparateur.cond);
      free_ast(a->comparateur.interne);
      free_ast(a->next);
      free(a);
      break;  
    case AST_ELSE:
      free_ast(a->comparateur.interne);
      free_ast(a->next);
      free(a);
      break;
    case AST_INT:
    case AST_DOUBLE:
      free_ast(a->next);
      free(a);
      break;
    case AST_OP_INCR:
    case AST_OP_DECR:
    free_ast(a->operation.left);
    free_ast(a->next);
    free(a);
    break;
    case AST_FOR:
      free_ast(a->boucle_for.first);
      free_ast(a->boucle_for.second);
      free_ast(a->boucle_for.third);
      free_ast(a->boucle_for.interne);
      free_ast(a->next);
      free(a);
      break;
      default:
        fprintf(stderr,"unknow ast type\n");
        free_ast(a->next);
        free(a);
        break;
  }
  }
}

void ast_to_code(ast* a){
  ast* parcours = a;
  FILE* fichier = NULL;
  fichier = fopen("res_c.c", "w");
  if (fichier != NULL){
    ast_to_code_recur(parcours,fichier);
    fclose(fichier); 
  }    
}

void ast_to_code_recur(ast* a, FILE* fichier){
      if (a!=NULL){
        switch(a->type){
          case AST_FCT:
            fprintf(fichier,"%s %s(){\n",ast_type_to_string1(a->fonction.returnType),a->fonction.id);
            ast_to_code_recur(a->fonction.interne,fichier);
            fputs(";\nreturn 0;\n}\n", fichier);
            ast_to_code_recur(a->next,fichier);
            break; 
          case AST_ID:
            if (a->type_int.init)
              fprintf(fichier,"%sint ",a->type_int.constant?"const ":"");
            fprintf(fichier, "%s", a->type_int.id);
            if(a->type_int.value!=NULL){
              fputs(" = ", fichier);
              ast_to_code_recur(a->type_int.value,fichier);
            }
            if (a->next!=NULL)
              fputs(";\n", fichier);
            ast_to_code_recur(a->next,fichier);
            break;
          case AST_INT:
            fprintf(fichier, "%d",(int) a->number);
            break;
          case AST_DOUBLE:
            fprintf(fichier, "%lf", a->number);
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
            case AST_OP_INCR:
              ast_to_code_recur(a->operation.left,fichier);
              fputs("+1",fichier);
            break;
          case AST_OP_DECR:
            ast_to_code_recur(a->operation.left,fichier);
            fputs("-1",fichier);
          break;
          case AST_COND:
            ast_to_code_recur(a->condition.left,fichier);
            if(strcmp(a->condition.op,"true")==0)
              fprintf(fichier, " != 0");
            else if(strcmp(a->condition.op,"false")==0)
              fprintf(fichier, " == 0");
            else
              fprintf(fichier, " %s ", a->condition.op);
            ast_to_code_recur(a->condition.right,fichier);
            break;
          case AST_ELSE_IF:
            fputs("else ",fichier);          
          case AST_IF:
            fputs("if (",fichier);          
            ast_to_code_recur(a->comparateur.cond,fichier);            
            fputs("){\n",fichier);
            ast_to_code_recur(a->comparateur.interne,fichier);
            fputs(";}\n",fichier);        
            ast_to_code_recur(a->next,fichier);
            break;   
          case AST_ELSE:
            fputs("else\n ",fichier);          
            fputs("{\n",fichier);
            ast_to_code_recur(a->comparateur.interne,fichier);
            fputs(";}\n",fichier);        
            ast_to_code_recur(a->next,fichier);
            break; 
          case AST_FOR:
            fputs("for (",fichier);
            ast_to_code_recur(a->boucle_for.first,fichier);
            fputs(";",fichier);
            ast_to_code_recur(a->boucle_for.second,fichier);
            fputs(";",fichier);
            ast_to_code_recur(a->boucle_for.third,fichier);
            fputs(")\n{",fichier);
            ast_to_code_recur(a->boucle_for.interne,fichier);
            fputs("}\n",fichier);            
            ast_to_code_recur(a->next,fichier);
            break; 
        }
    }
}

  /*int main(){
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
  }*/