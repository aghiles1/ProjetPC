#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

#include "define.h"
#include "util.h"
#include "personne.h"
#include "case.h"
#include "portion.h"

#define UNDEFINED_MOD -1
#define T0_MOD 0
#define T1_MOD 1

void do_tzero(int mesure, int nb);

void do_tun(int mesure, int nb);

int main(int argc, char** argv) {

	int nbp = -1, mode = UNDEFINED_MOD;
	int mesure = 0;

	//vérification des arguments
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-p")==0){
			i++;
			if(i >= argc)
				break;
			nbp = atoi(argv[i]);
			if(nbp > 9 || nbp < 0){
				fprintf(stderr, "%s", "ERREUR PARAMETRE -p [0-9] EN DEHORS DE LA PLAGE\n");
				exit(1);
			}
			continue;
		}
		if(strcmp(argv[i], "-t0")==0){
			mode = T0_MOD;
			continue;
		}
		if(strcmp(argv[i], "-t1")==0){
			mode = T1_MOD;
			continue;
		}
		if(strcmp(argv[i], "-m")==0){
			mesure = 1;
			continue;
		}
	}

	if (nbp < 0){
		fprintf(stderr, "%s", "Argument -p suivi de [0-9] non présent ou en dehors de la plage de donnees acceptee\n");
		exit(1);
	}
	if (mode == UNDEFINED_MOD){
		fprintf(stderr, "%s", "Aucun mode choisi ! (-t[01])\n");
		exit(1);
	}

  int nb = (int)pow(2, nbp);

	if(mode == T0_MOD){
		do_tzero(mesure, nb);
	}
	else if(mode == T1_MOD){
		do_tun(mesure, nb);
	}

	return 0;
}


void do_tzero(int mesure, int nb){
	pthread_t* tab_threads = (pthread_t*) malloc(sizeof(pthread_t)*nb);
	personne* personnes = (personne*) malloc(sizeof(personne)*nb);

	//création de la matrice du jeu (lignes)
	t_case*** matrice = (t_case***) malloc(sizeof(t_case**)*HEIGHT);
	for (int i = 0; i < HEIGHT; ++i){
		matrice[i] = (t_case**) malloc(sizeof(t_case*)*WIDTH);
	}

	int it = (mesure==1) ? 5 : 1;
	for(int exec = 0; exec < it; exec++){
		for (int i = 0; i < HEIGHT; ++i)
			for (int j = 0; j < WIDTH; ++j)
				matrice[i][j] = create_case();
	//récupérer un tableau de personnes
		init_matrice_personnes(matrice, nb, personnes, NULL);
		//récupérer un tableau de PID de threads
		create_threads_personnes(tab_threads, nb, personnes);
		//attendre la fin des thread avant que le programme s'arrete
		for (int i = 0; i < nb; i++)
			pthread_join(tab_threads[i], NULL);

		for (int i = 0; i < HEIGHT; ++i)
			for (int j = 0; j < WIDTH; ++j)
				free_case(matrice[i][j]);
	}
}


void do_tun(int mesure, int nb){
	pthread_t* tab_threads = (pthread_t*) malloc(sizeof(pthread_t)*4);
	personne* personnes = (personne*) malloc(sizeof(personne)*nb);
	t_portion** portions = (t_portion**) malloc(sizeof(t_portion*)*4);

	//création de la matrice du jeu (lignes)
	t_case*** matrice = (t_case***) malloc(sizeof(t_case**)*HEIGHT);
	for (int i = 0; i < HEIGHT; ++i){
		matrice[i] = (t_case**) malloc(sizeof(t_case*)*WIDTH);
	}

	int it = (mesure==1) ? 5 : 1;
	for(int exec = 0; exec < it; exec++){
		for (int i = 0; i < HEIGHT; ++i)
			for (int j = 0; j < WIDTH; ++j)
				matrice[i][j] = create_case();

		portions[0] = create_portion(0, 63, 0, 255, nb);
		portions[1] = create_portion(64, 127, 0, 255, nb);
		portions[2] = create_portion(0, 63, 256, 511, nb);
		portions[3] = create_portion(64, 127, 256, 511, nb);

		//récupérer un tableau de personnes
		init_matrice_personnes(matrice, nb, personnes, portions);
		//récupérer un tableau de PID de threads
		create_threads_portions(tab_threads, portions);
		//attendre la fin des thread avant que le programme s'arrete
		for (int i = 0; i < 4; i++)
			pthread_join(tab_threads[i], NULL);

		for (int i = 0; i < HEIGHT; ++i)
			for (int j = 0; j < WIDTH; ++j)
				free_case(matrice[i][j]);

		free_portion(portions[0]);
		free_portion(portions[1]);
		free_portion(portions[2]);
		free_portion(portions[3]);
	}
}
