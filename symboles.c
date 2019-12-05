
#include "symboles.h"

symboles new_table() {
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
  symboles s_same = getSymbole(s, id); 
  if(s_same!=NULL && portee!=s_same->portee){
    while (s_same->pnext!=NULL){
      s_same = s_same->suivant; 
    }
    symboles new = malloc(sizeof(symboles*));
    new->id = id;
    new->portee = portee;
    s_same->pnext = new;
    return s_same;
  }
  return NULL;
}

symboles add_symbole(symboles s,char * id, int portee){
  symboles s_same = getSymbole(s, id); 
    if (add_portee(s_same,id,portee)==NULL){
      symboles new = malloc(sizeof(struct symboles));
      new->id = id;
      new->portee = portee;
      new->pnext = NULL;
      new->suivant = s;
     return new;
    }
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


int main(){
  symboles s = new_table();
  s = add_symbole(s,"hello",0);
  s = add_symbole(s,"hopla",3);
  s = add_symbole(s,"hello",5);
  print_table(s);
  return 0;
}
