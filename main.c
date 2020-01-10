#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include <unistd.h>
#include <getopt.h>

extern int parse();
extern int parseFile();

void version(){
    printf("################\nMembres du projet\nMonfouga Marie\nRauch Arthur\nHuynh Jimmy\nBauer Emilien\n################\n");
}

int main(int argc,char* const argv[]){
    int opt;
    int print_ast=0;
    int print_tab=0;
    char* filename;
    int option_index=0;
    static struct option long_options[] = {
        {"version",      no_argument,       0,  'v' },
        {"tos", no_argument,       0,  't' },
        {"ast",    no_argument, 0,  'a' },
        {"output",   required_argument, 0,  'o' },
        {0,           0,                 0,  0   }
    };

    while((opt = getopt_long_only(argc, argv, "qvtao:",long_options,&option_index)) != -1){
        switch (opt)
        {
        case 'q':
            freopen("/dev/null","w",stdout);
            break;
        case 'v':
            version();
            break;
        case 'a':
            print_ast=1;
            break;
        case 't': 
            print_tab=1;
            break;
        case 'o':
            filename=optarg;
            printf("output file is: %s\n",filename);
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
        fprintf(stderr,"Impossible d'ouvrir le fichier code_c.c\n");
    }
    return res;
}