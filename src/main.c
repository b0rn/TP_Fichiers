#include <stdlib.h>
#include <stdio.h>
#include "fichier.h"

int main (int argc, char **argv){
  struct Fichier *f1;
  struct Fichier *f2;
  char t[2048] = {0};

  if (argc != 3){
    printf("Error \n");
    exit(EXIT_FAILURE);
  }

  printf("Ouverture de %s en écriture\n",argv[1]);
  f1 = ouvrir(argv[1], 'E');
  if (f1 == NULL) {
    printf("Error in First File opening \n");
    exit(EXIT_FAILURE);
  }

  printf("Ouverture de %s en lecture\n",argv[2]);
  f2 = ouvrir(argv[2], 'L');
  if (f2 == NULL) {
    printf("Error in Second File opening \n");
    exit(EXIT_FAILURE);
  }

  printf("Lecture de %d caractères :\n",lire(&t,1,300,f2));
  printf("%s\n", t);

  printf("Écriture de %d caractères : \n",ecrire(&t,1,300,f1));

  int i = fermer(f1)+fermer(f2);
  printf("Fermer : %d \n", i);

  return EXIT_SUCCESS;
}
