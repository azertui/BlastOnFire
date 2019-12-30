
#include "symboles.h"

table new_table()
{
  table new = malloc(sizeof(table));
  new->next = NULL;
  new->pred = NULL;
  new->available = NULL;
  return new;
}

symboles new_symboles(char *id, int constant)
{
  symboles new = malloc(sizeof(symboles));
  new->id = strndup(id, strlen(id));
  new->constant = constant;
  new->next = NULL;
  return new;
}

int analyse_ast_aux(ast *a, table t)
{
  static int ident = 0;
  if (a != NULL)
  {
    for (int k = 0; k < ident; k++)
      printf("\t");
    switch (a->type)
    {
    case AST_FCT:
      add_symbole(t, new_symboles(a->fonction.id, 0));
      add_table(t);
      ident++;
      if (analyse_ast_aux(a->fonction.interne, t))
        return 1;
      pop_table(t);
      ident--;
      break;
    case AST_OP_PLUS:
    case AST_OP_MOINS:
    case AST_OP_MUL:
    case AST_OP_DIV:
    case AST_OP_MODULO:
      if (analyse_ast_aux(a->operation.right, t))
        return 1;
    case AST_OP_INCR:
    case AST_OP_DECR:
      if (analyse_ast_aux(a->operation.left, t))
        return 1;
      break;
    case AST_COND:
      if (analyse_ast_aux(a->condition.left, t))
        return 1;
      if (analyse_ast_aux(a->condition.right, t))
        return 1;
      break;
    case AST_IF:
    case AST_ELSE_IF:
      if (analyse_ast_aux(a->comparateur.cond, t))
        return 1;
    case AST_ELSE:
      add_table(t);
      ident++;
      if (analyse_ast_aux(a->comparateur.interne, t))
        return 1;
      pop_table(t);
      ident--;
      break;
    case AST_ID:
      if (a->type_int.init)
      {
        add_symbole(t, new_symboles(a->type_int.id, a->type_int.constant));
      }
      else
      {
        if (find_symbole(t, a->type_int.id) == NULL)
        {
          fprintf(stderr, "Unknown reference to %s\n", a->type_int.id);
          return 1;
        }
      }
      if(analyse_ast_aux(a->type_int.value,t))
        return 1;
      break;
    default:
      break;
    }
    if (analyse_ast_aux(a->next, t))
      return 1;
  }
  return 0;
}

int analyse_ast(ast *a)
{
  printf("analyse en cours...\n");
  table t = new_table();
  int res = analyse_ast_aux(a, t);
  free_table(t);
  return res;
}

table add_table(table t)
{
  if (t == NULL)
  {
    return NULL;
  }
  table pointer = t;
  while (pointer->next != NULL)
  {
    pointer = pointer->next;
  }
  pointer->next = new_table();
  pointer->next->pred = pointer;
  return pointer->next;
}

void free_symboles(symboles s)
{
  symboles pointer = s;
  symboles tmp = NULL;
  while (pointer != NULL)
  {
    tmp = pointer->next;
    free(pointer->id);
    free(pointer);
    pointer = tmp;
  }
}

void free_table(table t)
{
  if (t != NULL)
  {
    if (t->next != (table)NULL)
    {
      fprintf(stderr, "free_table: not the latest table\n");
    }
    free_symboles(t->available);
    free(t);
  }
}

void pop_table(table t)
{
  table last = t;
  table pred = NULL;
  while (last->next != NULL)
  {
    pred = last;
    last = last->next;
  }
  free_table(last);
  if (pred != NULL)
    pred->next = NULL;
}

symboles find_symbole(table t, char *id)
{
  printf("search symbol %s\n", id);
  if (t == NULL)
    return NULL;
  table last = t;
  while (last->next != NULL)
  {
    last = last->next;
  }
  while (last != NULL)
  {
    for (symboles ps = last->available; ps != NULL; ps = ps->next)
    {
      if (ps->id != NULL && strcmp(ps->id, id) == 0)
        return ps;
    }

    last = last->pred;
  }
  return NULL;
}

void add_symbole(table tab, symboles s)
{
  printf("add %s\n", s->id);
  if (tab == NULL)
  {
    fprintf(stderr, "add_symbole:empty table\n");
  }
  table t = tab;
  while (t->next != NULL)
  {
    t = t->next;
  }
  if (t->available == NULL)
  {
    t->available = s;
  }
  else
  {
    symboles pointer = t->available;
    while (pointer->next != NULL)
    {
      if (strcmp(s->id, pointer->id) == 0)
      {
        fprintf(stderr, "Erreur: redeclaration de %s\n", s->id);
      }
      pointer = pointer->next;
    }
    pointer->next = s;
  }
}

/*symboles new_table() {
	return (symboles)NULL;
}


symboles getSymbole(symboles s, char* id){
  while(s!=NULL){
    if(strcmp(id,s->id)==0){
      return s;
    }
    s = s->suivant;
  }
  return NULL;
}

symboles add_portee(symboles s,char * id, int portee){
  if(s!=NULL){
    if(find_portee(s,portee)==NULL){
      while (s->pnext!=NULL){
       s = s->pnext; 
      }
      symboles new = malloc(sizeof(symboles*));
      new->id = strndup(id,strlen(id));
      new->portee = portee;
      new->pnext = NULL;
      s->pnext = new;
      return s;
    }
  }
  return NULL;
}

symboles add_symbole(symboles s,char * id, int portee,int constant){
  symboles s_same = getSymbole(s, id);
    if (s_same == NULL){
      symboles new = malloc(sizeof(struct symboles));
      new->id = strndup(id,strlen(id));
      new->portee = portee;
      new->constant=constant;
      new->pnext = NULL;
      new->suivant = s;
     return new;
    }
    else
      add_portee(s_same,id,portee);
  return s;  
}

void print_table(symboles s){
  symboles s_same = s; 
  while(s!=NULL){
    while (s_same!=NULL){
    printf("id : %s, portée: %d\n",s_same->id, s_same->portee);
    s_same = s_same->pnext;
    }
    s = s->suivant;
    s_same = s;
  }
}

void free_symboles(symboles s){
  if (s!=NULL){
    free_symboles(s->pnext);
    free_symboles(s->suivant);
    if(s->id!=NULL)
      free(s->id);
    free(s);
  }
}

*/