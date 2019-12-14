#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
extern int parse();
extern int parseFile();

int main(){

    FILE* fichier = NULL;
    fichier = fopen("code_c.c", "r");
    
    if (fichier != NULL)
    {
        parseFile(fichier);        
        fclose(fichier);     
    }
    else
    {
        printf("Impossible d'ouvrir le fichier code_c.c\n");
    }
}