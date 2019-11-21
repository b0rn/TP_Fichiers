#include <stdlib.h>
#include <stdio.h>
#include "fichier.h"

int main (int argc, char **argv){
  // struct Fichier *f1;
  // struct Fichier *f2;
  // char t[2048] = {0};
  //
  // if (argc != 3){
  //   printf("Error \n");
  //   exit(EXIT_FAILURE);
  // }
  //
  // printf("Ouverture de %s en écriture\n",argv[1]);
  // f1 = ouvrir(argv[1], 'E');
  // if (f1 == NULL) {
  //   printf("Error in First File opening \n");
  //   exit(EXIT_FAILURE);
  // }
  //
  // printf("Ouverture de %s en lecture\n",argv[2]);
  // f2 = ouvrir(argv[2], 'L');
  // if (f2 == NULL) {
  //   printf("Error in Second File opening \n");
  //   exit(EXIT_FAILURE);
  // }
  //
  // printf("Lecture de %d caractères :\n",lire(&t,1,300,f2));
  // printf("%s\n", t);
  // printf("Lecture de %d caractères :\n",lire(&t,1,91600,f2));
  // printf("%s\n",t);
  //
  // printf("Écriture de %d caractères : \n",ecrire(&t,1,300,f1));
  //
  // int i = fermer(f1)+fermer(f2);
  // printf("Fermer : %d \n", i);

  Fichier *f1,*f2;
  char c;
  if(argc != 3) return EXIT_FAILURE;
  f1 = ouvrir(argv[1],'L');
  printf("%s ouvert en lecture\n",argv[1]);
  if(f1 == NULL) return EXIT_FAILURE;
  f2 = ouvrir(argv[2],'E');
  printf("%s ouvert en écriture\n",argv[2]);
  if(f2 == NULL) return EXIT_FAILURE;

  while(lire(&c,1,1,f1) == 1)
    ecrire(&c,1,1,f2);

  printf("Tentative de lecture de 4 caractères après la fin du fichier...\n");
  char tab[5] = "";
  printf("Lecture de %d caractères\n",lire(&tab,1,4,f1));
  tab[4] = '\0';
  printf("tab = %s\n",tab);

  fermer(f1);
  printf("%s fermé\n",argv[1]);
  fermer(f2);
  printf("%s fermé\n",argv[2]);

  return EXIT_SUCCESS;
}
