# Projet compilation

-- Rauch Arthur
-- Monfouga Marie
-- Bauer Emilien
-- Huynh Jimmy

Dépôt Github: 
https://github.com/azertui/BlastOnFire
Lien vers la documentation en ligne:
https://azertui.github.io/BlastOnFire/Doxygen/html/index.html

## Compte rendu des fonctionnalités du projet.

### Options

* version (v): Affiche le noms des différents membres du projet.
* tos (t): Affiche la table des symboles(portée des ids par bloc)
* ast (a): Affiche l'AST intégral généré à partir du code source fourni en entrée (défaut: fichier code_c.c)
* file (f): Permet de spécifier le nom du fichier source en entrée.
* output(o):Permet de spécifier le nom du fichier source en sortie.
* q: Permet de rendre le programme silencieux en désactivant la sortie sur le terminal.

### Syntaxe supportée

L'analyseur syntaxique est capable de reconnaître et de générer un AST à partir des éléments suivants:

* Fonctions avec ou sans paramètres ou retours, dont le corps est défini avec des accolades mais peut être vide.
* Appels de fonctions avec ou sans paramètres ou retours.
* Opérateurs + - * / %
* Opérateurs d'affectation += -= /=*= %=
* Opérateurs unaires: ++ --
* Opérateurs de comparaison: a\<b a\>b a==b a<=b a>=b a!=b !a a
* Déclaration de variables entières et de tableaux d'entiers à n dimentions int tab[5][9]...
* Déclaration de variables flottantes et de tableaux de flottants à n dimentions double tab[5][9]...
* Commentaires (effacés, absents à la sortie)
* Variables constantes
* Structures conditionnelles if, else if, else
* Boucles for, avec possibilité de paramètres nuls: for(;i<5;i++) 
* Boucles while

### Vérification de la portée des identifiants

On utilise une table des symboles structurée en portée: Chaque bloc est associé à une table, et chacune de ces tables est elle-même stockée dans une pile globale.
Quand on rencontre un nouveau bloc, on empile une nouvelle table. Si l'on rencontre un identifiant, on vérifie sa portée (défini dans la table courante ou les tables parentes précédentes de la pile) . Quand le bloc se termine, on dépile ladite table.