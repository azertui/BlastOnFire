
typedef struct symboles {
    int init;
    char* id;
    //value = sous ast correspondant
    struct symboles *suivant;
} symboles;

symboles* new_symbole(symboles*, symboles*);
symboles* symbole_new_id(char*);
void symboles_print(symboles*);
