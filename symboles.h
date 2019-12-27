#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct symboles 
{
    int portee;
    int constant;
    char* id;
    struct symboles *pnext;
    struct symboles *suivant;
}test, *symboles;

symboles new_table();

symboles getSymbole(symboles s, char* id);

symboles add_symbole(symboles s,char * id, int portee,int constant);

symboles change_symbole(symboles s,char * id, int portee);

symboles find_portee(symboles s,int portee);

symboles add_portee(symboles s,char * id, int portee);

void print_table(symboles s);

//TODO
void free_symboles(symboles s);
