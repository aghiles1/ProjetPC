#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

#include "define.h"
#include "matrice.h"
#include <time.h>

#define T0_MOD false
#define T1_MOD true

int main(int argc, char** argv) {

	int nbp;
	bool mode, mesure = false;
	clock_t t1,t2;
	//vérification des arguments
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-p")==0){
			nbp = atoi(argv[++i]);
			if(nbp > 9 || nbp < 0){
				std::cerr << "ERREUR PARAMETRE -p [0-9] EN DEHORS DE LA PLAGE" << std::endl;
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
			mesure = true;
			continue;
		}
	}

	pthread_t* tid = (pthread_t*) malloc(sizeof(pthread_t)*(int)pow(2, nbp));
	Per* personnes = static_cast<Per*> (malloc(sizeof(Per)*(int)pow(2, nbp)));

	if(mode == T0_MOD){
		int tab_mesures[5] = {};
		//création de la matrice du jeu (lignes)
		int** matrice_jeu = static_cast<int**> (malloc(sizeof(int*)*HEIGHT));
		if(matrice_jeu == NULL){
			std::cerr << "ERREUR DE GENERATION DE LA MATRICE" << std::endl;
			exit(1);
		}
		//création de la matrice du jeu (colonnes)
		for (int i = 0; i < HEIGHT; i++){
			*(matrice_jeu+i) = static_cast<int*> (malloc(sizeof(int)*WIDTH));
			if(*(matrice_jeu+i) == NULL){
				std::cerr << "ERREUR DE GENERATION DE LA MATRICE" << std::endl;
				exit(1);
			}
		}
		//nombre de personnes
		int nb = (int)pow(2, nbp);
		int it = (mesure) ? 5 : 1;
		for(int exec = 0; exec < it; exec++){
		//récupérer un tableau de personnes
			init(matrice_jeu,nb,personnes);
			//récupérer un tableau de PID de threads
			create_threads_personnes(personnes,nb,tid);
			//attendre la fin des thread avant que le programme s'arrete
			for (int i = 0; i < nb; i++){
		       		if(pthread_join(tid[i], NULL) != 0)
					printf("bug_join\n");
			};
			
		}
		std::cout << "Mesure : MOYENNE DES MESURES" << std::endl;
	}
	else if(mode == T1_MOD){
		std::cout << "Fonction non implémentée..." << std::endl;
	}
	return 0;
}

