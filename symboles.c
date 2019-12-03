#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"

symboles* new_symbole(symboles all,symboles new) {
  new->suivant = all;
  return new;
}


symboles* symbole_new_id(char* id) {
  //check existence
  symboles* new = malloc(sizeof(symboles));
  new->init = 0;
  new->id = strdup(id);
  new->suivant = NULL;
  return new;
}

void symboles_print(symboles* s) {
  while (s!=NULL)
      printf("%s\n",s->id);
      s = s->suivant;
  }  
}
