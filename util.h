#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <SDL/SDL.h>
#include <X11/Xlib.h>

#include "personne.h"
#include "case.h"
#include "portion.h"

void create_threads_personnes(pthread_t* tab, int nb, personne* personnes);

void create_threads_portions(pthread_t* tab, t_portion** portions);

void init_matrice_personnes(t_case*** matrice, int p, personne* personnes, t_portion** portions);

void affiche(t_case***);

void* deplacer_zero(void* p);

void* deplacer_un(void* p);

void init_sdl(void);

void remplir(int,int,SDL_Surface*,Uint32);

void remplir1(int,int,SDL_Surface*,Uint32);

#endif
