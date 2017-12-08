
#include "portion.h"

void free_portion(t_portion* p){
  free(p->personnes);
  free(p);
}

t_portion* create_portion(int xmin, int xmax, int ymin, int ymax, int nb_max_personne){
  t_portion* p = (t_portion*) malloc(sizeof(t_portion));
  p->xmin = xmin;
  p->xmax = xmax;
  p->ymin = ymin;
  p->ymax = ymax;
  p->nb_personnes = 0;

  p->personnes = (personne**) malloc(sizeof(personne**)*nb_max_personne);

  for(int i=0; i<nb_max_personne; i++)
    p->personnes[i] = NULL;

  return p;
}
