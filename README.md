# Parties codées
* Interface Fichier :
  * Fichier* ouvrir(char *nom, char mode);
  * int fermer(Fichier *f);
  * int lire(void *p, unsigned int taille, unsigned int nbelem, Fichier *f);
  * int ecrire(void *p, unsigned int taille, unsigned int nbelem, Fichier *f);
  * int fecriref(Fichier *f, char *format, ...);
  * int fliref(Fichier *f, char *format, ...);
