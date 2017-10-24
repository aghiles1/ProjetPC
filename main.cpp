#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "matrice.h"
#include "thread_personne.h"
#include <iostream>

#include <math.h>
#include <string.h>


int main(int argc, char** argv) {

	int nbp;
	bool mode, mesure = false;

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

	int nb = (int)pow(2, nbp);

	int** matrice_jeu = init(HEIGHT, WIDTH, nb);
	affiche(matrice_jeu,HEIGHT,WIDTH);
	return 0;

}

