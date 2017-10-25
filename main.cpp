#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <iostream>

#include <sys/wait.h>
#include <math.h>
#include <string.h>

#include "define.h"
#include "matrice.h"

int main(int argc, char** argv) {

	int nbp;
	bool mode, mesure = false;
	/**
		
	**/
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-p")==0){
			nbp = atoi(argv[++i]);
			if(nbp > 9 || nbp < 0);
				std::cerr << "ERREUR PARAMETRE -p [0-9] EN DEHORS DE LA PLAGE" << std::endl;
			continue;
		}
		if(strcmp(argv[i], "-t0")==0){
			mode = false;
			continue;
		}
		if(strcmp(argv[i], "-t1")==0){
			mode = true;
			continue;
		}
		if(strcmp(argv[i], "-m")==0){
			mesure = true;
			continue;
		}
	}

	int** matrice_jeu = static_cast<int**> (malloc(sizeof(int*)*HEIGHT));
	if(matrice_jeu == NULL){
		std::cerr << "ERREUR DE GENERATION DE LA MATRICE" << std::endl;
		exit(1);
	}
	for (int i = 0; i < HEIGHT; i++){
		*(matrice_jeu+i) = static_cast<int*> (malloc(sizeof(int)*WIDTH));
		if(*(matrice_jeu+i) == NULL){
			std::cerr << "ERREUR DE GENERATION DE LA MATRICE" << std::endl;
			exit(1);
		}
	}

	int nb = (int)pow(2, nbp);

	Per* personnes = init(matrice_jeu,nb);
	pthread_t* tid = create_threads_personnes(personnes,nb);
	for (int i = 0; i < nb; i++)
       pthread_join(tid[i], NULL);
   	affiche(matrice_jeu,HEIGHT,WIDTH);
	delete[] personnes;

	return 0;
}

