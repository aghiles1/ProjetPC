
#include "case.h"

void free_case(t_case* c){
  free(c);
}
/**
 * [create_case est une fonction qui crée une case de notre matrice]
 * @return [un pointeur sur une t_case]
 */
t_case* create_case(){
  t_case* c = (t_case*) malloc(sizeof(t_case));
  CASE_STATE(c) = EMPTY;
  if (pthread_mutex_init(&CASE_VERROU(c), NULL) != 0){
    fprintf(stderr, "%s", "Error init mutex...\n");
    exit(1);
  }
  return c;
}
