#include <stdlib.h>
#include <stdio.h>
#include "fichier.h"

int main(int argc, char **argv){
  Fichier *f1, *f2;

  if(argc != 2) return EXIT_SUCCESS;

  f1 = ouvrir(argv[1],'E');
  if(f1 == NULL) return EXIT_FAILURE;

  printf("%d elems formatés écrits\n",fecriref(f1," %c %s 12\n",'a', "bonjour"));
  printf("%d elems formatés écrits\n",fecriref(f1," %d \n",-324978));
  fermer(f1);

  f2 = ouvrir(argv[1],'L');
  if(f2 == NULL) return EXIT_FAILURE;
  char c,s[50];
  int d, out;
  out = fliref(f2," %c %s %d\n",&c,s,&d);
  printf("J'ai lu %d elems : %c %s %d\n",out,c,s,d);
  out = fliref(f2," %d \n%s",&d,s);
  printf("J'ai lu %d elems : %d %s\n",out,d,s);
  fermer(f2);

  return EXIT_SUCCESS;
}
