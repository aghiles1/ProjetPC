
#ifndef _PERSONNE_H_
#define _PERSONNE_H_

#include "case.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct personne personne;

struct personne {
  int x;
  int y;
  pthread_mutex_t verrou;
  t_case*** matrice;
};

#endif
