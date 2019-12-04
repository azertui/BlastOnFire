-Décision de se restreindre au BLAS :

  Ca fait déjà un travail sérieux mais permet de simplifier le problème, donc d'éspérer un résultat satisfaisant.

Informations nécessaires pour chaque fonction :

 - Nom
 - Type
 - Paramètres (types)
 - Instruction(s)
 - Domaines d'éxécutions (Fonctions des paramètres dimensionnels)

On remarquera que pour le niveau 0, les domaines d'éxécutions sont inutiles (une seule éxécution de l'instruction car pas de boucle)
On peut donc commencer par définir une grammaire et les structures de données associées simplifiées pour ce cas :

  Nom, Type, Paramètres, Instruction.


On peut proposer une grammaire du type :

  fonction     : type NOM paramètres '\n' instructions

  type         : INT
               | DOUBLE

  paramètres   : type paramètres
               | ε

  instructions : instruction intructions
               | ε

  instruction  : domaine corps

  domaine      : ε {A modifier avec les niveaux supérieurs}

  corps        : { Ici il faudra ce qui correspond à une instruction dans le langage parsé }
