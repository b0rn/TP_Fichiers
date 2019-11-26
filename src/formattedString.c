#include <stdlib.h>
#include <stdio.h>
#include "fichier.h"

int main(int argc, char **argv){
  Fichier *f1, *f2;

  if(argc != 2) return EXIT_SUCCESS;

  f1 = ouvrir(argv[1],'E');
  if(f1 == NULL) return EXIT_FAILURE;

  fecriref(f1," %c %s 12\n",'a', "bonjour");
  fecriref(f1," %d \n",-324978);

  fermer(f1);

  return EXIT_SUCCESS;
}
