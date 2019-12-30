#ifndef AST_H 
#define AST_H
/**
 * \file ast.h
 * \brief Header de l'arbre de syntaxe
 * \author Monfouga Marie
 * \version 0.2
 * \date 11 décembre 2019
 *
 * Header de l'arbre de syntaxe
 *
 */

/**
 * \enum ast_type
 * \brief types.
 *
 * ast_type est une série de types qui pourront être stockés dans l'arbre et reconnu par yacc
 */

typedef enum { AST_ID, AST_INT,AST_DOUBLE, AST_OP_PLUS, AST_OP_MUL, AST_OP_MODULO, AST_OP_MOINS, AST_OP_DIV, AST_FCT, AST_IF, AST_ELSE_IF ,AST_ELSE,AST_COND, AST_OP_INCR,AST_OP_DECR} ast_type;
/** 
 * \struct ast
 * \brief Noeud de l'ast.
 *
 * Un noeud de l'ast est défini par un certain type et quelques caractéristiques
 * dépendantes du type de noeud.  
 */
typedef struct ast {
 ast_type type; /*!< Type du noeud */
  union {
    struct {
      struct ast* left;
      struct ast* right; 
    } operation;
    struct {
      char* id; /*!< identificateur */
      struct ast* value;  /*!< noeud de valeur*/
      int init; /*!< booléen pour l'initialisation*/
      int constant;/*!< boolean pour définir si la variable est une constante*/
    } type_int; /*!< variable, constante */
    double number;
    char* id;
    struct {
      char* op;
      struct ast* left;
      struct ast* right;
    } condition;
    struct {
      struct ast* cond;
      struct ast* interne;  
    } comparateur;
    struct {
      struct ast* interne;  
      char* id;
      ast_type returnType;
    } fonction;
    } ;
  struct ast* next; /*!< Pointeur vers un autre noeud, la suite de l'arbre*/
  unsigned int uid;
} ast;

/**
 * \fn void attribute_uid(ast* a);
 * \brief Permet d'attribuer un id unique à un contenant
 * 
 * \param a ast se voyant attribuer un uid
 */
void attribute_uid(ast* a);

/**
 * \fn ast* ast_new_main_fct(ast* next);
 * \brief Fonction de création d'une nouvelle instance de type "fonction principale d'un programme C" d'un objet ast.
 *
 * \param next ast à attacher au nouveau noeud ou NULL.
 * \return Instance nouvelle allouée d'un objet de type ast.
 */
ast* ast_new_main_fct(ast* body,ast* next,char* id,ast_type returnType);

ast* new_ast_fake();

/**
 * \fn ast* ast_new_operation(enum ast_type, ast*, ast*);
 * \brief Fonction de création d'un noeud faisant office d'une opération arithmétique
 *
 * \param type type de l'opération ayant lieu entre les arbres.
 * \param left ast gauche de l'opération.
 * \param right ast droite de l'opération.
 * \return Ast de l'opération.
 */
ast* ast_new_operation(ast_type type, ast* left, ast* right);

/**
 * \fn ast* ast_new_number(int);
 * \brief Fonction de création d'un noeud d'un int
 *
 * \param number valeur flottante ou entiere.
 * \param is_int booléen vrai si le nombre est un entier
 * \return Noeud du nombre.
 */
ast* ast_new_number(double number, int is_int);

/**
 * \fn ast* ast_new_id(char* id,ast* value);
 * \brief Fonction de création d'un noeud d'un identificateur
 *
 * \param id nom de l'identificateur.
 * \param value valeur associée à l'ID, null si pas initialisée.
 * \param init 1 si initialisation, 0 sinon.
 * \return Ast de l'id.
 */
ast* ast_new_id(char* id,ast* value, int init, int constant);

/**
 * \fn ast* ast_double_to_integer(ast* number);
 * \brief Fonction spécifiant qu'un ast de nombre est de type entier
 * 
 * \param number ast du nombre devant être converti
 * \return l'ast du nombre converti ou NULL en cas d'erreur
 */
ast* ast_double_to_integer(ast* number);

/**
 * \fn ast* ast_new_condition(ast* left, ast* right, char* op, ast_type);
 * \brief Fonction de création d'une nouvelle condition.
 *
 * \param left ast gauche de l'opération.
 * \param right ast droite de l'opération.
 * \param op operation entre les arbres
 * \return Instance nouvelle allouée d'un objet de type ast.
 */
ast* ast_new_condition(ast* left, ast* right, char* op);

/**
 * \fn ast* ast_new_comparateur(ast* cond,ast* interne, ast_type);
 * \brief Fonction de création d'une nouvelle condition.
 *
 * \param cond ast de la condition.
 * \param interne ast présent dans le if
 * \param type type de l'opération ayant lieu entre les arbres.
 * \return Instance nouvelle allouée d'un objet de type ast.
 */
ast* ast_new_comparateur(ast* cond,ast* interne, ast_type type);



/**
 * \fn void ast_print(ast* ast, int indent);
 * \brief Fonction d'affichage d'un objet ast.
 *
 * \param ast ast à afficher.
 * \param indent indentation à créer avant de print.
 */
void ast_print(ast* ast, int indent);


/**
 * \fn ast* ast_link(ast* a, ast* next);
 * \brief Fonction reliant le dernier noeud d'un ast à un objet ast.
 *
 * \param a arbre.
 * \param next arbre.
 * \return Arbre a dont le dernier élément next pointe sur l'arbre.
*/
ast* ast_link(ast* a, ast* next);

/**
 * \fn void free_ast(ast* ast);
 * \brief Libération de l'arbre
 *
 * \param ast arbre.
*/
void free_ast(ast* ast);

/**
 * \fn void ast_to_code(ast* a);
 * \brief Transformation de l'arbre en code C dans un fichier res_c.c
 *
 * \param ast arbre.
*/
void ast_to_code(ast* a);

/**
 * \fn void ast_to_code_recur(ast* a, FILE* fichier);
 * \brief Fonction auxiliaire de ast_to_code
 *
 * \param ast arbre.
 * \param fichier fichier où écrire le code.
*/
void ast_to_code_recur(ast* a, FILE* fichier);

#endif