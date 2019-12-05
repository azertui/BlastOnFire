#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct symboles 
{
    int portee;
    char* id;
    struct symboles *pnext;
    struct symboles *suivant;
}test, *symboles;

symboles new_table();

symboles add_symbole(symboles s,char * id, int portee);

symboles change_symbole(symboles s,char * id, int portee);

void print_table(symboles s);

symboles getSymbole(symboles s, char* id);
