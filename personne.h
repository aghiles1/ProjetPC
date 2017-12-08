
#ifndef _PERSONNE_H_
#define _PERSONNE_H_

#include "case.h"

typedef struct personne personne;

struct personne {
  int x;
  int y;
  t_case*** matrice;
};

#endif
