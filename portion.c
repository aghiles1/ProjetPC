
#include "portion.h"

void free_portion(t_portion* p){
  free(p);
}
/**
 * [create_portion sert a crée une portion sur la matrice]
 * @param  xmin            [description]
 * @param  xmax            [description]
 * @param  ymin            [description]
 * @param  ymax            [description]
 * @param  nb_max_personne [description]
 * @return                 [un pointeur sur la portion]
 */
t_portion* create_portion(int xmin, int xmax, int ymin, int ymax, int nb_max_personne){
  t_portion* p = (t_portion*) malloc(sizeof(t_portion));
  p->xmin = xmin;
  p->xmax = xmax;
  p->ymin = ymin;
  p->ymax = ymax;
  p->nb_personnes = nb_max_personne;

  p->personnes = (personne*) malloc(sizeof(personne*)*nb_max_personne);

  return p;
}
