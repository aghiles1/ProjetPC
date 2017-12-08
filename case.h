
#ifndef _CASE_H_
#define _CASE_H_

#include "define.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct s_case t_case;

struct s_case {
  int state;
  pthread_mutex_t verrou;
};

#define CASE_STATE(c)        ((c)->state)
#define CASE_VERROU(c)       ((c)->verrou)

t_case* create_case();

void free_case(t_case* c);

#endif
