#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

typedef struct symboles {
    int portee;
    char* id;

    struct symboles *suivant;
} *symboles;

symboles new_table();

symboles add_symbole(symboles s,char * id, int portee);

symboles change_symbole(symboles s,char * id, int portee);

symboles print_table(symboles s);

symboles getSymbole(symboles s, char* id, int p);
