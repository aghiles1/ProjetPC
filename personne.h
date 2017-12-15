
#ifndef _PERSONNE_H_
#define _PERSONNE_H_

#include "case.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct personne personne;
/**
 * personne est une structure qui represente une personne sur le terrain avec :
 * x et y qui vont representer la position de la persnne sur la matrice
 * verrou pour verrouiller la personne au moment du deplacement d'une portion à l'autre
 * matrice un pointeur vers la matrice (terrain)
 */
struct personne {
  int x;
  int y;
  pthread_mutex_t verrou;
  t_case*** matrice;
};

#endif
