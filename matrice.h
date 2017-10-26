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

/**
 * [create_threads_personnes crée les threads qui vont faire avancer les personnes vers la sortie et initialise le Mutex]
 * @param  tab [contient les personnes pour savoir leur positions]
 * @param  nb  [le nombre de personnes présente sur la matrice]
 * @return     [les PID de chaque thread qui vont servir pour les attendre jusqua la fin du main]
 */
void create_threads_personnes(Per* tab,int nb, pthread_t* personnes);

/**
 * [init la fonction init initialise la matrice du jeu qui représente le terrain. Les personnes sont généré aléatoirement.]
 * @param  matrice_jeu [une matrice sur laquelle les personnes, obstacles et sortie sont reprisanté]
 * @param  p           [le nombre de personnes présente sur la matrice]
 * @return             [tableau de pointeurs vers personnes]
 */
void init(int** matrice_jeu,int p,Per* personnes);

/**
 * [affiche affiche la matrice]
 * @param matrice_jeu [une matrice sur laquelle les personnes, obstacles et sortie sont reprisanté]
 * @param hight       [La hauteur de la matrice (le nombre de lignes)]
 * @param width       [La largeur de la matrice (le nombre de collone)]
 */
void affiche(int** matrice_jeu,int hight,int width);

/**
 * [deplacer est une fonction qui sert a deplacer le personnage sur la matrice est qui les conduit a la sortie. Cette fonction est appelé par tout les thread créer]
 * @param p [un pointeur vers la personne qu'il faut deplacer]
 */
void *deplacer(void* p);

#endif
