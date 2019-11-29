# Parties codées
* Interface Fichier :
  * Fichier* ouvrir(char *nom, char mode);
  * int fermer(Fichier *f);
  * int lire(void *p, unsigned int taille, unsigned int nbelem, Fichier *f);
  * int ecrire(void *p, unsigned int taille, unsigned int nbelem, Fichier *f);
  * int fecriref(Fichier *f, char *format, ...);
  * int fliref(Fichier *f, char *format, ...);

# Architecture
* bin/ : dossier contenant les exécutables; Les exécutables *.static ont été compilés avec la bibliothèque statique, les exécutables *.dynamic ont été compilés avec la bibliothèque dynamique, les autres ont été compilés sans bibliothèque
* include/ : dossier contenant les headers
* src/ : dossier contenant les sources
* obj/ : dossier contenant les fichiers .o
* lib/ : dossier contenant les bibliothèques générées

# Tests réalisés
Les différentes versions compilées (avec bibliothèque statique/dynamique et sans bibliothèque) sont fonctionnellement identiques.
## Programme *main*
* Ce programme prend 2 fichiers en arguments et copie l'intégralité du 1er fichier dans le 2eme fichier. Ensuite il tente de lire 4 caractères après la fin du 1er fichier, juste pour vérifier que ce n'est pas possible.
* Utilisation : ***main fichier1 fichier2***

## Programme *formattedString*
* Ce programme prend un fichier vide en argument. Il y écrit deux chaines formatées et tente par la suite de lire deux chaines formatées à partir de ce même fichier.

## Utilisation du Makefile
* make clean : supprime tout sauf les sources/headers
* make all : compile les programmes (toutes les variantes)
* make deleteTexts : supprime les fichiers textes
* make withoutLibTest : lance les tests sur les programmes compilés sans bibliothèque
* make staticLibTest : lance les tests sur les programmes compilés avec la bibliothèque statique
* make dynamicLibTest : lance les tests sur les programmes compilés avec la bibliothèque dynamique

Proposition de tests :
```
make clean
make all
make withoutLibTest
make deleteTexts
make staticLibTest
make deleteTexts
make dynamicLibTest
```
