#ifndef MATRICE_H
#define MATRICE_H

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

struct Personne {
	int x;
	int y;
};
typedef Personne* Per;


pthread_t* create_threads_personnes(Per* tab,int nb);

Per* init(int** matrice_jeu,int p);
void affiche(int** matrice_jeu,int hight,int width);

#endif
