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
  f->fd = (f->mode == 'E')?open(nom, O_WRONLY):open(nom, O_RDONLY);

  if(f->fd == -1){
    free(f);
    free(t);
    return NULL;
  }

  if(mode == 'L'){
    if(loadReadBuffer(f) == -1){
      free(f);
      free(t);
      return NULL;
    }
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
  unsigned int out = 0;
  unsigned int toRead = (buffer->head + taille * nbelem > buffer->tail)?buffer->tail-buffer->head:taille*nbelem;
  while(toRead != 0){
    memcpy(p+out*sizeof(char),buffer->buffer + buffer->head*sizeof(char),toRead);
    //*((char*)(p+(out+toRead)*sizeof(char))) = '\0';
    f->offset += toRead;
    out += toRead;

    if(buffer->head + toRead == buffer->size){
      buffer->head = 0;
      loadReadBuffer(f);
    }else
      buffer->head += toRead;
    toRead = ((taille * nbelem - out) >= buffer->tail)?buffer->tail:taille * nbelem - out;
  }
  return out;
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

int fecriref(Fichier *f, char *format, ...){
    va_list vl;
    va_start(vl,format);
    char parsing = 0;
    size_t formatLength = strlen(format);
    char c;
    char *s;
    int d, count = 0;
    for(int i = 0; i < formatLength; i++){
      if(format[i] == '%') parsing = 1;
      else{
        if(parsing == 1){
          switch(format[i]){
            case 'c':
              c = (char)va_arg(vl,int);
              ecrire(&c,1,1,f);
              count++;
            break;
            case 's':
              s = va_arg(vl,char*);
              ecrire(s,1,strlen(s),f);
              count++;
            break;
            case 'd':
              d = va_arg(vl,int);
              char t[35] = "";
              int offset = 0;
              intToString(d,t,&offset);
              ecrire(t,1,strlen(t),f);
              count++;
            break;
          }
          parsing = 0;
        }else
          ecrire(&(format[i]),1,1,f);
      }
    }
    va_end(vl);
    return count;
}

int fliref(Fichier *f, char *format, ...){
  va_list vl;
  va_start(vl,format);
  char parsing = 0;
  size_t formatLength = strlen(format);
  char *cPtr,cVal, cTmp = 0;
  char *sPtr;
  int *intPtr, count = 0, lu = 0, addedC = 0;
  for(int i = 0; i < formatLength; i++){
    if(format[i] == '%') parsing = 1;
    else{
      if(parsing == 1){
        switch(format[i]){
          case 'c':
            cPtr = (char*)va_arg(vl,char*);
            if(cTmp != 0){
              *cPtr = cTmp;
              lu = 1;
              cTmp = 0;
            }else
              lu = lire(cPtr,1,1,f);
            if(lu == 0) return count;
            addedC = 1;
          break;
          case 's':
            sPtr = va_arg(vl,char*);
            addedC = 0;
            if(cTmp != 0){
              *sPtr = cTmp;
              addedC++;
              cTmp = 0;
            }
            lu = lire(&cVal,1,1,f);
            while(lu != 0 && cVal != '\n' && cVal != ' '){
              *(sPtr+addedC*sizeof(char)) = cVal;
              lu = lire(&cVal,1,1,f);
              addedC++;
            }
            *(sPtr+addedC*sizeof(char)) = '\0';
            if(lu != 0) cTmp = cVal;
          break;
          case 'd':
            intPtr = va_arg(vl,int*);
            char t[35] = "";
            addedC = 0;
            if(cTmp != 0){
              *t = cTmp;
              addedC++;
              cTmp = 0;
            }
            lu = lire(&cVal,1,1,f);
            while(lu != 0 && (cVal == '-' || (cVal >= '0' && cVal <= '9' && cVal != '-'))){
              if(addedC != 34){
                *(t+addedC*sizeof(char)) = cVal;
                addedC++;
              }
              lu = lire(&cVal,1,1,f);
            }
            if(lu != 0) cTmp = cVal;
            *(t+addedC*sizeof(char)) = '\0';
            *intPtr = strtol(t,NULL,10);
          break;
        }
        if(addedC != 0) count++;
        if(lu == 0) return count;
        parsing = 0;
        addedC = 0;
      }else{
        if(cTmp != 0){
          cVal = cTmp;
          cTmp = 0;
        }else
          lire(&cVal,1,1,f);
        if(cVal != format[i]) return count;
      }
    }
  }
  va_end(vl);
  return count;
}

size_t loadReadBuffer(Fichier *f){
  size_t tmp = read(f->fd,f->buffer->buffer,f->buffer->size);
  if(tmp < f->buffer->size) f->buffer->tail = tmp;
  else if(tmp != -1) f->buffer->tail = f->buffer->size;
  return tmp;
}

void intToString(int i,char *dest, int *offset){
  if(i < 0){
    char c = '-';
    memcpy(dest+*offset*sizeof(char),&c,1);
    *offset = *offset+1;
    *(dest+*offset*sizeof(char)) = '\0';
    i *= -1;
  }
  if(i/10 > 0)
    intToString(i/10,dest,offset);
  char c = '0' + i%10;
  memcpy(dest+*offset*sizeof(char),&c,1);
  *offset = *offset + 1;
  *(dest+*offset*sizeof(char)) = '\0';
}
