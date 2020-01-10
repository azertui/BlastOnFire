/**
 * \file ast.c
 * \brief Source de l'arbre de syntaxe
 * \author Monfouga Marie
 * \version 3.0
 * \date 03 janvier 2020
 *
 * Code source des fonctions de l'arbre de syntaxe
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define YYERROR_VERBOSE 1

ast *ast_new_main_fct(ast *body, ast *next, char *id, ast_type returnType)
{
  ast *new = malloc(sizeof(ast));
  new->type = AST_FCT;
  attribute_uid(new);
  new->fonction.id = strndup(id, strlen(id));
  new->fonction.interne = body;
  new->fonction.returnType = returnType;
  new->next = next;
  return new;
}

ast *ast_new_app(char* id, int nb_param, ast** params)
{
    ast* new = malloc(sizeof(ast));
    new->type = AST_APP;
    new->appel.id = id;
    new->appel.nb_param = nb_param;
    new->appel.params = params;
    new->next = NULL;

    return new;
}

ast *ast_new_operation(ast_type type, ast *left, ast *right)
{
  ast *new = malloc(sizeof(ast));
  new->type = type;
  new->operation.left = left;
  new->operation.right = right;
  new->next = NULL;
  return new;
}

int nb_dim(array a)
{
  int res = 0;
  if (a != NULL)
  {
    while (a->next != NULL)
    {
      res++;
      a = a->next;
    }
  }
  return res;
}

void free_arr(array a)
{
  if (a != NULL)
  {
    array p=a;
    array next=a->next;
    while (next!=NULL)
    {
      free(p);
      p=next;
      next=next->next;
    }
    free(p);
  }
}

ast *ast_new_number(double number, int is_int)
{
  ast *new = malloc(sizeof(ast));
  new->type = AST_DOUBLE;
  if (is_int)
    new->type = AST_INT;
  new->number = number;
  new->next = NULL;
  return new;
}

ast *ast_new_id(char *id, ast *value, int init, int constant, int is_int)
{
  ast *new = malloc(sizeof(ast));
  new->type = AST_ID;
  new->type_int.id = strndup(id, strlen(id));
  new->type_int.value = value;
  new->type_int.init = init;
  new->type_int.constant = constant;
  new->type_int.is_int=is_int;
  new->next = NULL;
  return new;
}

ast *ast_new_tab_int(char *id, ast *value, int init, array nb, int constant)
{
  ast *new = malloc(sizeof(ast));
  new->type = AST_INT_TAB;
  new->type_int_tab.id = strndup(id, strlen(id));
  new->type_int_tab.value = value;
  new->type_int_tab.init = init;
  new->type_int_tab.constant = constant;
  new->type_int_tab.nb_elem = nb_dim(nb);
  new->type_int_tab.nb = nb;
  new->next = NULL;
  return new;
}

void attribute_uid(ast *a)
{
  static unsigned int count = 1;
  a->uid = count++;
}

char *ast_type_to_string1(ast_type t)
{
  char *tab[(AST_WHILE - AST_ID) + 1] = {"AST_ID", "int" ,"AST_INT_TAB","double", "AST_OP_PLUS", "AST_OP_MUL", "AST_OP_MODULO", "AST_OP_MOINS", "AST_OP_DIV", "AST_FCT", "AST_APP", "AST_IF", "AST_ELSE_IF" ,"AST_ELSE","AST_COND", "AST_OP_INCR","AST_OP_DECR", "AST_FOR", "AST_WHILE"};
  return tab[t];
}

ast *ast_double_to_integer(ast *number)
{
  if (number->type == AST_INT)
  {
    return number;
  }
  if (number->type != AST_DOUBLE)
  {
    fprintf(stderr, "ast_double_to_integer: not an integer type\n");
    return NULL;
  }
  number->number = (int)number->number;
  number->type = AST_INT;
  return number;
}

ast *ast_new_condition(ast *left, ast *right, char *op)
{
  ast *new = malloc(sizeof(ast));
  new->type = AST_COND;
  new->condition.left = left;
  new->condition.right = right;
  if (op != NULL)
    new->condition.op = strndup(op, strlen(op));
  new->next = NULL;
  return new;
}

ast *ast_new_comparateur(ast *cond, ast *interne, ast_type type)
{
  ast *new = malloc(sizeof(ast));
  new->type = type;
  attribute_uid(new);
  if (type == AST_IF || type == AST_ELSE_IF)
  {
    new->comparateur.cond = cond;
  }
  new->comparateur.interne = interne;
  new->next = NULL;
  return new;
}

ast *ast_new_boucle_for(ast *first, ast *second, ast *third, ast *interne)
{
  ast *new = malloc(sizeof(ast));
  new->type = AST_FOR;
  attribute_uid(new);
  new->boucle_for.first = first;
  new->boucle_for.second = second;
  new->boucle_for.third = third;
  new->boucle_for.interne = interne;
  new->next = NULL;
  return new;
}

ast *ast_new_boucle_while(ast *cond, ast *interne)
{
  ast *new = malloc(sizeof(ast));
  new->type = AST_WHILE;
  attribute_uid(new);
  new->boucle_while.cond = cond;
  new->boucle_while.interne = interne;
  new->next = NULL;
  return new;
}

void ast_print(ast *ast, int indent)
{
  if (ast != NULL)
  {
    for (int i = 0; i < indent; i++)
      printf("    ");
    switch (ast->type)
    {
    case AST_FCT:
      printf("FCT (%s) type:%s\n", ast->fonction.id, ast_type_to_string1(ast->fonction.returnType));
      ast_print(ast->fonction.interne, indent + 1);
      ast_print(ast->next, indent);
      break;
    case AST_APP:
      printf("fct (%s)\n", ast->appel.id);
      if(ast->appel.nb_param)
      {
          for(int i = ast->appel.nb_param-1; i > 0; i--)
              ast_print(ast->appel.params[i], indent + 1);
          ast_print(ast->appel.params[0], indent + 1);
      }
      ast_print(ast->next, indent);
      break;
    case AST_ID:
      printf("ID (%s)%s", ast->type_int.id, ast->type_int.constant ? ": const" : "");
      if (ast->type_int.value != NULL)
      {
        printf(" = \n");
        ast_print(ast->type_int.value, indent + 1);
      }
      else
        printf("\n");

      ast_print(ast->next, indent);
      break;
    case AST_INT_TAB:
      printf("ID (%s)%s", ast->type_int_tab.id, ast->type_int_tab.constant ? ": const" : "");
      array b = ast->type_int_tab.nb;
      while (b != NULL)
      {
        printf("[%d]", b->n_dim);
        b = b->next;
      }
      if (ast->type_int_tab.value != NULL)
      {
        printf(" = \n");
        ast_print(ast->type_int_tab.value, indent + 1);
      }
      else
        printf("\n");

      ast_print(ast->next, indent);
      break;
    case AST_DOUBLE:
      printf("NUMBER %s (%lf)\n", ast_type_to_string1(ast->type), ast->number);
      break;
    case AST_INT:
      printf("NUMBER %s (%d)\n", ast_type_to_string1(ast->type), (int)ast->number);
      break;
    case AST_OP_PLUS:
      printf("+ \n");
      ast_print(ast->operation.left, indent + 1);
      ast_print(ast->operation.right, indent + 1);
      ast_print(ast->next, indent);
      break;
    case AST_OP_MUL:
      printf("* \n");
      ast_print(ast->operation.left, indent + 1);
      ast_print(ast->operation.right, indent + 1);
      ast_print(ast->next, indent);
      break;
    case AST_OP_MOINS:
      printf("- \n");
      ast_print(ast->operation.left, indent + 1);
      ast_print(ast->operation.right, indent + 1);
      ast_print(ast->next, indent);
      break;
    case AST_OP_DIV:
      printf("/ \n");
      ast_print(ast->operation.left, indent + 1);
      ast_print(ast->operation.right, indent + 1);
      ast_print(ast->next, indent);
      break;
    case AST_COND:
      if (ast->condition.op != NULL)
        printf("%s", ast->condition.op);
      printf("\n");
      ast_print(ast->condition.left, indent + 1);
      ast_print(ast->condition.right, indent + 1);
      break;
    case AST_ELSE_IF:
      printf("ELSE ");
    case AST_IF:
      printf("IF : \n");
      ast_print(ast->comparateur.cond, indent + 1);
      ast_print(ast->comparateur.interne, indent + 1);
      ast_print(ast->next, indent);
      break;
    case AST_ELSE:
      printf("ELSE : \n");
      ast_print(ast->comparateur.interne, indent + 1);
      ast_print(ast->next, indent);
      break;
    case AST_OP_INCR:
      printf("++\n");
      break;
    case AST_OP_DECR:
      printf("--\n");
      break;
    case AST_FOR:
      printf("FOR \n");
      ast_print(ast->boucle_for.first, indent + 1);
      ast_print(ast->boucle_for.second, indent + 1);
      ast_print(ast->boucle_for.third, indent + 1);
      ast_print(ast->boucle_for.interne, indent + 1);
      ast_print(ast->next, indent);
      break;
    case AST_WHILE:
      printf("WHILE \n");
      ast_print(ast->boucle_while.cond, indent + 1);
      printf("\n");
      ast_print(ast->boucle_while.interne, indent + 1);
      ast_print(ast->next, indent);
      break;
    default:
      fprintf(stderr, "print_ast:Unknown ast type:%d\n", ast->type);
    }
  }
}

ast *ast_link(ast *a, ast *next)
{
  ast *test = a;
  while (test->next != NULL)
  {
    test = test->next;
  }
  test->next = next;
  return a;
}

void free_ast(ast *a)
{
  if (a != NULL)
  {
    switch (a->type)
    {
    case AST_FCT:
      free(a->fonction.id);
      free_ast(a->fonction.interne);
      free_ast(a->next);
      free(a);
      break;
    case AST_APP:
      free(a->appel.id);
      if(a->appel.nb_param)
      {
          for(int i = 0; i < a->appel.nb_param; i++)
              free_ast(a->appel.params[i]);
      }
      free_ast(a->next);
      free(a);
      break;
    case AST_ID:
      free_ast(a->type_int.value);
      free(a->type_int.id);
      free_ast(a->next);
      free(a);
      break;
    case AST_INT_TAB:
      free_ast(a->type_int.value);
      free(a->type_int.id);
      free_ast(a->next);
      free_arr(a->type_int_tab.nb);
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
      if (a->condition.op != NULL)
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
    case AST_WHILE:
      free_ast(a->boucle_while.cond);
      free_ast(a->boucle_while.interne);
      free_ast(a->next);
      free(a);
      break;
    default:
      fprintf(stderr, "unknow ast type\n");
      free_ast(a->next);
      free(a);
      break;
    }
  }
}

void ast_to_code(ast *a)
{
  ast *parcours = a;
  FILE *fichier = NULL;
  fichier = fopen("res_c.c", "w");
  if (fichier != NULL)
  {
    ast_to_code_recur(parcours, fichier);
    fclose(fichier);
  }
}

void ast_to_code_recur(ast *a, FILE *fichier)
{
  if (a != NULL)
  {
    switch (a->type)
    {
    case AST_FCT:
      fprintf(fichier, "%s %s(){\n", ast_type_to_string1(a->fonction.returnType), a->fonction.id);
      ast_to_code_recur(a->fonction.interne, fichier);
      fputs(";\nreturn 0;\n}\n", fichier);
      ast_to_code_recur(a->next, fichier);
      break;
    case AST_APP:
      fprintf(fichier, "%s(", a->appel.id);
      if(a->appel.nb_param)
      {
          for(int i = a->appel.nb_param-1; i > 0; i--)
          {
              ast_to_code_recur(a->appel.params[i], fichier);
              fprintf(fichier, ", ");
          }
          ast_to_code_recur(a->appel.params[0], fichier);
      }
      fprintf(fichier, ");\n");
      ast_to_code_recur(a->next, fichier);
      break;
    case AST_ID:
      if (a->type_int.init)
        fprintf(fichier, "%s%s ", a->type_int.constant ? "const " : "",a->type_int.is_int?"int":"double");
      fprintf(fichier, "%s", a->type_int.id);
      if (a->type_int.value != NULL)
      {
        fputs(" = ", fichier);
        ast_to_code_recur(a->type_int.value, fichier);
      }
      if (a->next != NULL)
        fputs(";\n", fichier);
      ast_to_code_recur(a->next, fichier);
      break;
    case AST_INT_TAB:
      if (a->type_int_tab.init)
        fprintf(fichier, "%sint ", a->type_int_tab.constant ? "const " : "");
      fprintf(fichier, "%s", a->type_int_tab.id);
      array b = a->type_int_tab.nb;
      while (b != NULL)
      {
        fprintf(fichier, "[%d]", b->n_dim);
        b = b->next;
      }

      if (a->type_int.value != NULL)
      {
        fputs(" = ", fichier);
        ast_to_code_recur(a->type_int.value, fichier);
      }

      if (a->next != NULL)
        fputs(";\n", fichier);
      ast_to_code_recur(a->next, fichier);
      break;
    case AST_INT:
      fprintf(fichier, "%d", (int)a->number);
      break;
    case AST_DOUBLE:
      fprintf(fichier, "%lf", a->number);
      break;
    case AST_OP_PLUS:
      ast_to_code_recur(a->operation.left, fichier);
      fputs(" + ", fichier);
      ast_to_code_recur(a->operation.right, fichier);
      break;
    case AST_OP_MOINS:
      ast_to_code_recur(a->operation.left, fichier);
      fputs(" - ", fichier);
      ast_to_code_recur(a->operation.right, fichier);
      break;
    case AST_OP_MUL:
      ast_to_code_recur(a->operation.left, fichier);
      fputs(" * ", fichier);
      ast_to_code_recur(a->operation.right, fichier);
      break;
    case AST_OP_DIV:
      ast_to_code_recur(a->operation.left, fichier);
      fputs(" / ", fichier);
      ast_to_code_recur(a->operation.right, fichier);
      break;
    case AST_OP_INCR:
      ast_to_code_recur(a->operation.left, fichier);
      fputs("+1", fichier);
      break;
    case AST_OP_DECR:
      ast_to_code_recur(a->operation.left, fichier);
      fputs("-1", fichier);
      break;
    case AST_COND:
      ast_to_code_recur(a->condition.left, fichier);
      if (strcmp(a->condition.op, "true") == 0)
        fprintf(fichier, " != 0");
      else if (strcmp(a->condition.op, "false") == 0)
        fprintf(fichier, " == 0");
      else
        fprintf(fichier, " %s ", a->condition.op);
      ast_to_code_recur(a->condition.right, fichier);
      break;
    case AST_ELSE_IF:
      fputs("else ", fichier);
    case AST_IF:
      fputs("if (", fichier);
      ast_to_code_recur(a->comparateur.cond, fichier);
      fputs("){\n", fichier);
      ast_to_code_recur(a->comparateur.interne, fichier);
      fputs(";}\n", fichier);
      ast_to_code_recur(a->next, fichier);
      break;
    case AST_ELSE:
      fputs("else\n ", fichier);
      fputs("{\n", fichier);
      ast_to_code_recur(a->comparateur.interne, fichier);
      fputs(";}\n", fichier);
      ast_to_code_recur(a->next, fichier);
      break;
    case AST_FOR:
      fputs("for (", fichier);
      ast_to_code_recur(a->boucle_for.first, fichier);
      fputs(";", fichier);
      ast_to_code_recur(a->boucle_for.second, fichier);
      fputs(";", fichier);
      ast_to_code_recur(a->boucle_for.third, fichier);
      fputs(")\n{", fichier);
      ast_to_code_recur(a->boucle_for.interne, fichier);
      fputs(";}\n", fichier);
      ast_to_code_recur(a->next, fichier);
      break;
    case AST_WHILE:
      fputs("while (", fichier);
      ast_to_code_recur(a->boucle_while.cond, fichier);
      fputs(")\n{", fichier);
      ast_to_code_recur(a->boucle_while.interne, fichier);
      fputs(";}\n", fichier);
      ast_to_code_recur(a->next, fichier);
      break;
    }
  }
}
