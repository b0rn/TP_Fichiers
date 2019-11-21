#include "fichier.h"

Fichier *ouvrir(char *nom, char mode){
  if(mode != 'E' && mode != 'L') return NULL;

  Fichier *f;
  Tampon *t;
  f = (Fichier*) malloc(sizeof(Fichier));
  t = (Tampon*) malloc(sizeof(Tampon));

  t->size = SIZE;
  t->buffer[0] = '\0';
  t->head = 0;
  t->tail = 0;
  f->mode = mode;
  f->buffer = t;
  f->offset = 0;
  f->size = 0;
  f->fd = (f->mode == 'E')?open(nom, O_WRONLY):open(nom, O_RDONLY);

  if(f->fd == -1){
    free(f);
    free(t);
    return NULL;
  }

  return f;
}

int fermer(Fichier *f) {
  if(f->mode == 'E' && f->buffer->tail != 0)
    write(f->fd,f->buffer->buffer,f->buffer->tail);
  if(close(f->fd) == -1)
      return -1;
  free(f->buffer);
  free(f);
  return 0;
}

int lire(void *p, unsigned int taille, unsigned int nbelem, Fichier *f){
  if(f->mode != 'L') return -1;

  Tampon *buffer = f->buffer;
  unsigned int read = 0;
  unsigned int toRead = (buffer->head + taille * nbelem > buffer->size)?buffer->size - buffer->head:taille * nbelem;
  char eof = 0;
  while(toRead != 0){
    if(!(buffer->head > buffer->tail || buffer->tail - buffer->head >= toRead || (buffer->head == buffer->tail && buffer->head != 0))){
      size_t readC = loadBuffer(f);
      if(readC == -1) return -1;
      if(toRead > buffer->tail - buffer->head + readC){
        toRead = buffer->tail - buffer->head + readC;
        eof = 1;
      }
    }

    memcpy(p+read*sizeof(char),buffer->buffer + buffer->head*sizeof(char),toRead);
    *((char*)(p+(read+toRead)*sizeof(char))) = '\0';

    f->offset += toRead;
    if(buffer->head + toRead == buffer->tail){
      buffer->head = 0;
      buffer->tail = 0;
    }else
      buffer->head = (buffer->head+toRead == buffer->size)?0:buffer->head+toRead;
    read += toRead;
    toRead = ((taille * nbelem - read) / buffer->size >= 1)?buffer->size:taille * nbelem - read;
    if(eof == 1) // reached EOF
      return read;
  }
  loadBuffer(f);
  return read;
}

int ecrire(void *p, unsigned int taille, unsigned int nbelem, Fichier *f){
  if(f->mode != 'E') return -1;

  Tampon *buffer = f->buffer;
  unsigned int wrote = 0;
  unsigned int toWrite = (buffer->tail + taille * nbelem > buffer->size)?buffer->size - buffer->tail:taille * nbelem;
  while(toWrite != 0){
      memcpy(buffer->buffer + buffer->tail*sizeof(char),p+wrote*sizeof(char),toWrite);
      if(buffer->tail + toWrite == buffer->size){
        buffer->tail = 0;
        write(f->fd,buffer->buffer,buffer->size);
      }else
        buffer->tail += toWrite;
      f->offset += toWrite;
      wrote += toWrite;
      toWrite = ((taille * nbelem - wrote) >= buffer->size)?buffer->size:taille * nbelem - wrote;
  }
  return wrote;
}

size_t loadBuffer(Fichier *f){
  Tampon *buffer = f->buffer;
  void *addr = buffer->buffer;
  size_t count = 0, rw = 0;
  if(buffer->head == buffer->tail && buffer->head == 0){
    // si le buffer est vide , tout remplir
    count = buffer->size;
    rw = read(f->fd,addr,count);
    if(rw == -1)return -1;
    buffer->tail = buffer->size;
  }
  else if(buffer->tail > buffer->head){
    // si la queue est après la tête
    char allocStart = 1;
    if(buffer->size - buffer->tail >= MIN_SIZE_IO){
      // si il reste de la place à la fin du tampon , on remplit
      addr += buffer->tail*sizeof(char);
      count = buffer->size - buffer->tail;
      rw = read(f->fd,addr,count);
      if(rw == -1)return -1;
      buffer->tail = buffer->size;
    }else if(buffer->tail != buffer->size)
      allocStart = -1;

    if(allocStart == 1 && buffer->head >= MIN_SIZE_IO){
      // si on peut remplir l'avant , on remplit
      addr = buffer->buffer;
      count = buffer->head;
      rw = read(f->fd,addr,count);
      if(rw == -1)return -1;
      buffer->tail = buffer->head;
    }
  }else if(buffer->head > buffer->tail && buffer->head - buffer->tail >= MIN_SIZE_IO){
    // si la tête est après la queue et que l'espace est suffisant entre les deux
    addr += buffer->tail*sizeof(char);
    count = buffer->head - buffer->tail;
    rw = read(f->fd,addr,count);
    if(rw == -1)return -1;
    buffer->tail = buffer->head;
  }
  return rw;
}
