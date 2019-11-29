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
## Programme *main*
  * Ce programme prend 2 fichiers en arguments et copie l'intégralité du 1er fichier dans le 2eme fichier. Ensuite il tente de lire 4 caractères après la fin du 1er fichier, juste pour vérifier que ce n'est pas possible.
  * Utilisation : ***main fichier1 fichier2***
## Programme *formattedString*
