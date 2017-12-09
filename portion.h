
#ifndef _PORTION_H_
#define _PORTION_H_

#include "define.h"
#include "personne.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <SDL/SDL.h>

typedef struct s_portion t_portion;

struct s_portion {
  int xmin;
  int xmax;
  int ymin;
  int ymax;
  int nb_personnes;
  Uint32 color;
  personne* personnes;
  t_case*** matrice;
};

t_portion* create_portion(int xmin, int xmax, int ymin, int ymax, int nb_max_personne);

void free_portion(t_portion* p);

#endif
