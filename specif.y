%{

%}

%%

fonction:
    type ID '(' parametres ')' '\n' INSTRUCTIONS_TAG '\n' '\n' '\n' instructions
    ;

type:
        INTEGER_T
    |   DOUBLE_T
    ;

parametres:
        type ',' parametres
    |   type
    ;

instructions:
        instruction '\n' '\n' instructions
    |   instruction
    ;

domaine:
/* A FAIRE POUR LES NIVEAUX >=1 */

corps:

%%
