#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include <unistd.h>
#include <getopt.h>

extern int parse();
extern int parseFile();

void version()
{
    printf("################\nMembres du projet\nMonfouga Marie\nRauch Arthur\nHuynh Jimmy\nBauer Emilien\n################\n");
}

int main(int argc, char *const argv[])
{
    int opt;
    int print_ast = 0;
    int print_tab = 0;
    char *filename = "res_c.c";
    char* input_filename= "code_c.c";
    int option_index = 0;
    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"tos", no_argument, 0, 't'},
        {"ast", no_argument, 0, 'a'},
        {"output", required_argument, 0, 'o'},
        {"file", required_argument, 0, 'f'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long_only(argc, argv, "qvtao:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 'q':
            freopen("/dev/null", "w", stdout);
            break;
        case 'v':
            version();
            break;
        case 'a':
            print_ast = 1;
            break;
        case 't':
            print_tab = 1;
            break;
        case 'o':
            filename = optarg;
            break;
        case 'f':
            input_filename=optarg;
            break;
        default:
            fprintf(stderr, "Unknow option %c\n", opt);
            printf("usage: %s -o [output_file] -version -tos -ast -q\n", argv[0]);
            return EXIT_FAILURE;
        }
    }
    printf("input file is: %s\n",input_filename);
    printf("output file is: %s\n", filename);
    FILE *fichier = NULL;
    fichier = fopen(input_filename, "r");
    int res = EXIT_FAILURE;
    if (fichier != NULL)
    {
        res = parseFile(fichier, NULL, print_ast, print_tab, filename);
        fclose(fichier);
    }
    else
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n",input_filename);
    }
    return res;
}