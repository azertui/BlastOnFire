/**
 * \file ast.h
 * \brief Header de l'arbre de syntaxe
 * \author Monfouga Marie
 * \version 0.1
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
enum ast_type { AST_ID, AST_NUMBER, AST_OP_PLUS, AST_OP_MUL, AST_OP_MOINS, AST_OP_DIV, AST_FCT};


/** 
 * \struct ast
 * \brief Noeud de l'ast.
 *
 * Un noeud de l'ast est défini par un certain type et quelques caractéristiques
 * dépendantes du type de noeud.  
 */
typedef struct ast {
  enum ast_type type; /*!< Type du noeud */
  union {
    struct {
      struct ast* left;
      struct ast* right; 
    } operation;
    struct {
      char* id; /*!< identificateur */
      struct ast* value;  /*!< noeud de valeur*/
    } type_int; /*!< variable, constante */
    int number;
    char* id;
  } ;
  struct ast* next; /*!< Pointeur vers un autre noeud, la suite de l'arbre*/
} ast;

/**
 * \fn ast* ast_new_main_fct(ast* next);
 * \brief Fonction de création d'une nouvelle instance de type "fonction principale d'un programme C" d'un objet ast.
 *
 * \param next ast à attacher au nouveau noeud ou NULL.
 * \return Instance nouvelle allouée d'un objet de type ast.
 */
ast* ast_new_main_fct(ast* next);


ast* ast_new_operation(enum ast_type, ast*, ast*);
ast* ast_new_number(int);
ast* ast_new_id(char*,ast*);

/**
 * \fn void ast_print(ast* ast, int indent);
 * \brief Fonction d'affichage d'un objet ast.
 *
 * \param ast ast à afficher.
 * \param indent indentation à créer avant de print.
 */
void ast_print(ast* ast, int indent);

ast* ast_link(ast* a, ast* next);

