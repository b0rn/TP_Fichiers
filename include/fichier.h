#ifndef __FICHIER_H__
#define __FICHIER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define SIZE 200
#define MIN_SIZE_IO 10

typedef struct Tampon {
  char buffer[SIZE];
  unsigned int size;
  unsigned int head;
  unsigned int tail;
}Tampon;

typedef struct Fichier {
  Tampon *buffer;
  char mode;
  int fd;
  unsigned int offset;
  unsigned int size;
}Fichier;

Fichier* ouvrir(char *nom, char mode);
int fermer(Fichier *f);
int lire(void *p, unsigned int taille, unsigned int nbelem, Fichier *f);
int ecrire(void *p, unsigned int taille, unsigned int nbelem, Fichier *f);

size_t loadBuffer(Fichier *f);

#endif

/*
cyclique variante : min de read/write max read/write=taille
*/
