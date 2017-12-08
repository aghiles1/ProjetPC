
#include "case.h"

void free_case(t_case* c){
  free(c);
}

t_case* create_case(int boolean_verrou){
  t_case* c = (t_case*) malloc(sizeof(t_case));
  CASE_STATE(c) = EMPTY;
  if(boolean_verrou==1)
    if (pthread_mutex_init(&CASE_VERROU(c), NULL) != 0){
      fprintf(stderr, "%s", "Error init mutex...\n");
      exit(1);
    }
  return c;
}
