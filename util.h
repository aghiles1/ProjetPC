#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "personne.h"
#include "case.h"


void create_threads_personnes(pthread_t* tab, int nb, personne* personnes);

void init_matrice_personnes(t_case*** matrice, int p, personne* personnes);

void affiche(t_case***);

void* deplacer(void* p);

#endif
