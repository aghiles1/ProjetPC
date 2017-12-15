
#ifndef _CASE_H_
#define _CASE_H_

#include "define.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct s_case t_case;
/**
 * structure s_case qui une case qui contient un attribut state qui indique ce qui ce trouve dans la case :
 * une personnes
 * un vide
 * ou un obstacle
 * L'attribut verrou est verrou qui sert a verouiller la case (s_case)
 */
struct s_case {
  int state;
  pthread_mutex_t verrou;
};

#define CASE_STATE(c)        ((c)->state)
#define CASE_VERROU(c)       ((c)->verrou)

t_case* create_case();

void free_case(t_case* c);

#endif
