
#include "symboles.h"

symboles new_table() {
	return (symboles)NULL;
}

symboles add_symbole(symboles s,char * id, int portee){
    symboles new = malloc(sizeof(symboles*));
    new->id = id;
    new->portee = portee;
    new->suivant = s;
    return new;
}

symboles print_table(symboles s){
  while(s!=NULL){
    printf("id : %s, portée: %d\n",s->id, s->portee);
    s = s->suivant;
  }
}

symboles getSymbole(symboles s, char* id, int p){
  while(s!=NULL){
    if(strcmp(id,s->id)==0){
      return s;
    }
    s = s->suivant;
  }
  return NULL;
}


int main(){
  symboles s = new_table();
  s = add_symbole(s,"hello",0);
  s = add_symbole(s,"hopla",3);
  print_table(s);
}
