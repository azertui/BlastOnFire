#include "detect.h"

int search_params_fun(char *key, ast *function)
{
    for(int i = 0; i < function->fonction.nb_param; i++)
    {
        if(strcmp(key, function->fonction.params[i]->id) == 0)
            return i;
    }
    return -1;
}

int search_params_app(char *key, ast *call)
{
    for(int i = 0; i < call->appel.nb_param; i++)
    {
        if(strcmp(key, call->appel.params[i]->id) == 0)
            return i;
    }
    return -1;
}

int mappable(char* var, char* param, ast* function, ast* call)
{
    int pos = search_params_fun(param, function);
    //Si, dansla fonction, c'est autre chose qu'un paramètre : faux a priori
    if(pos == -1)
        return 0;
    else{
        //Si le paramètre n'est pas encore mappé, on le map et c'est bon
        if(!call->appel.params[pos])
        {
            //VERIFIER TYPES ?
            call->appel.params[pos] = ast_new_id(var,
                                                 NULL, 0,
                                                 0, 0);
            return 1;
        //Si le paramètre est déjà mappé
        } else
            //On renvoie la correspondance ou non à la variable itérée
            return !strcmp(var, call->appel.params[pos]->type_int.id);
    }
}



ast* equivalent_snippet(ast* snippet, ast* function){
    return equivalent_snippet_recur(snippet, function->fonction.interne, function,
                                    ast_new_app(function->fonction.id,
                                                function->fonction.nb_param,
                                                calloc((function->fonction.nb_param + 1), sizeof(ast*))));
}


ast* equivalent_snippet_recur(ast *s, ast *fit, ast *function, ast *call)
{
  //Si on est au bout de la fonction : succès ! On se serait arrêté avant
  if(!fit)
      return call;
  //On est arrivé au bout du snippet mais pas de la fonction : échec
  if(!s)
      return NULL;
  //Si on est sur un return de la fonction
  if(fit->type == AST_RET)
  {
      //Ca doit être une assignation dans le bout de code
      if(s->type != AST_ID || !s->type_int.value)
          return NULL;
      //Est-ce qu'on a déjà un nom pour le retour de l'appel ?
      if(call->appel.params[call->appel.nb_param])
      {
          //Alors ça doit être le même
          if(strcmp(call->appel.params[call->appel.nb_param]->id, s->type_int.id))
              return NULL;
      }
      //Est-ce que les opérations sont les mêmes ?
          return equivalent_snippet_recur(s->type_int.value, fit->operation.right, function, call);

  }
  if(s->type != fit->type)
      return NULL;
  switch(fit->type)
  {
    case AST_ID:
      //Si seulement l'un est une définition
      if(s->type_int.init != fit->type_int.init)
          return NULL;
      //Si aucun n'est une affection
      else if(!s->type_int.value && !fit->type_int.value)
      {
          return mappable(s->type_int.id, fit->type_int.id, function, call);
      }
      //Si seulement l'un des deux est une affectation
      else if(!s->type_int.value || !fit->type_int.value)
          return NULL;
      //Si les deux sont des affectations
      else{
          //Est-ce qu'on peut mapper les variables affectées
          if(!mappable(s->type_int.id, fit->type_int.id, function, call))
              return NULL;
          //Est-ce qu'on peut mapper les valeurs affectées
          if(!equivalent_snippet_recur(s->type_int.value, fit->type_int.value, function, call))
              return NULL;
          //On continue
          return equivalent_snippet_recur(s->next, fit->next, function, call);
      }
      break;
      case AST_OP_PLUS:
      case AST_OP_MUL:
      case AST_OP_MODULO:
      case AST_OP_MOINS:
      case AST_OP_DIV:
      case AST_OP_DECR:
      case AST_OP_INCR:
        if(!equivalent_snippet_recur(s->operation.left, fit->operation.left, function, call)
         ||!equivalent_snippet_recur(s->operation.right, fit->operation.right, function, call))
            return NULL;
        else
            return call;
        break;
      default:
        return NULL;
  }
}





int equivalent_ast(ast *a, ast *b)
{
    table t = new_table();
    int res = equivalent_ast_recur(a, b, t);
    free_table(t);
    return res;
}

int equivalent_ast_recur(ast* a, ast* b, table t)
{
    int res;
    if(!a || !b)
        res =  0;
    if(a->type != b->type)
        res =  0;
    switch(a->type)
    {
    case AST_ID:
        //Si seulement l'un est une définition
        if(a->type_int.init != b->type_int.init)
            res =  0;
        //Si aucun n'est une affection
        else if(!a->type_int.value && !a->type_int.value)
            res =  a->type_int.is_int == b->type_int.is_int;
        //Si seulement l'un d'est deux
        else if(!a->type_int.value || !b->type_int.value)
            res =  0;
        //Si les deux sont des affectations
        else
            res =  equivalent_ast(a->type_int.value, b->type_int.value);
        break;
    case AST_INT:
    case AST_DOUBLE:
        res =  a->number == b->number;
        break;
    case AST_INT_TAB:
        res =  1;
        break;
    case AST_OP_PLUS:
    case AST_OP_MUL:
    case AST_OP_MODULO:
    case AST_OP_MOINS:
    case AST_OP_DIV:
    case AST_OP_DECR:
    case AST_OP_INCR:
        res =  equivalent_ast(a->operation.left, b->operation.left)
            && equivalent_ast(a->operation.right, b->operation.right);
        break;
    case AST_FCT:
        //Si les arités et types sont les mêmes
        if(a->fonction.nb_param == b->fonction.nb_param
                && a->fonction.returnType == b->fonction.returnType)
        {
            //On compare les corps et les paramètres
            for(int i = 0; i < a->fonction.nb_param; i++)
            {
                if(!equivalent_ast(a->fonction.params[i], b->fonction.params[i]))
                {
                    res =  0;
                    break;
                }
            }
            ast* ita = a->fonction.interne;
            ast* itb = b->fonction.interne;
            while(a && b)
            {
                if(!equivalent_ast(ita, itb))
                {
                    res =  0;
                    break;
                }
                a = a->next;
                b = b->next;
            }
            res =  1;
        }
        break;
    default:
        res =  0;
        break;
    }
    return res;
}
