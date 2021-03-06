#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "personne.h"
#include "case.h"
#include "portion.h"

void create_threads_personnes(pthread_t* tab, int nb, personne* personnes);

void create_threads_portions(pthread_t* tab, t_portion** portions);

void init_matrice_personnes(t_case*** matrice, int p, personne* personnes, t_portion** portions);

void affiche(t_case***);

void* deplacer_zero(void* p);

void* deplacer_un(void* p);

#endif
