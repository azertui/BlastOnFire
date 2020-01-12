#ifndef SYMBOLES_H
#define SYMBOLES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

typedef struct symboles
{
    int constant;
    char *id;
    struct symboles *next;
} test, *symboles;

typedef struct table
{
    int id;
    int indent;
    symboles available;
    struct table *next;
    struct table *pred;
} * table;

table new_table();

int analyse_ast(ast* a, int print_symb);

symboles new_symboles(char *id, int constant);

table add_table(table t, int print_symb);

void free_symboles(symboles s);

symboles find_symbole(table t, char* id);

void free_table(table t);

void pop_table(table t, int print_symb);

void add_symbole(table t, symboles s, int print_symb);
#endif