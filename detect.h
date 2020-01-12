#ifndef DETECT_H
#define DETECT_H

#include <stdio.h>
#include <string.h>
#include "symboles.h"
#include "ast.h"

//Renvoie la position de key dans la liste des paramètres ou -1 sinon
int search_params(char* key, ast* function);

//Renvoie l'appel équivalent ou NULL sinon
ast* equivalent_snippet(ast* snippet, ast* function);

//function : pour conservet, fit pour itérer, call pour construire l'appel petit à petit à renvoyer le cas échéant.
ast* equivalent_snippet_recur(ast* s, ast* fit, ast* function, ast* call);

#endif
