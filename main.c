#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include <unistd.h>
extern int parse();
extern int parseFile();

int main(int argc,char* const argv[]){
    int opt;
    while((opt = getopt(argc, argv, "q")) != -1){
        switch (opt)
        {
        case 'q':
            freopen("/dev/null","w",stdout);
            break;
        
        default:
            fprintf(stderr,"Unknow option %c\n",opt);
            return EXIT_FAILURE;
        }
    }

    FILE* fichier = NULL;
    fichier = fopen("code_c.c", "r");
    int res=EXIT_FAILURE;
    if (fichier != NULL)
    {
        res=parseFile(fichier,NULL);        
        fclose(fichier);     
    }
    else
    {
        printf("Impossible d'ouvrir le fichier code_c.c\n");
    }
    return res;
}