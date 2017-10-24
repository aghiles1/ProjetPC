#include "matrice.h"
#include <iostream>


int** init(int height, int width,int p){
	int** matrice_jeu = (int**) malloc(sizeof(int*)*height);
	if(matrice_jeu == NULL) 
		std::cerr << "ERREUR DE GENERATION DE LA MATRICE" << std::endl;

	if(p < 0 || p > 9){
		std::cerr << "ERROR (p=="<< p <<") : p doit étre dans l'interval suivant : [0,9]" << std::endl;
	}
	else {
		for (int i = 0; i < height-1; i++)
		{
			*(matrice_jeu+i) = (int*) malloc(sizeof(int)*width);
			if(*(matrice_jeu+i) == NULL) 
				std::cerr << "ERREUR DE GENERATION DE LA MATRICE" << std::endl;
			for (int j = 0; j < width-1; j++)
			{
				*(*(matrice_jeu+i)+j) = rand()%8;
			}
		}	
	}

	return matrice_jeu;
}

void affiche(int** matrice_jeu,int hight,int width){
	for (int i = 0; i < hight-1; ++i)
	{
		for (int j = 0; j < width-1; ++j)
		{
			std::cout << matrice_jeu[i][j];
		}
		std::cout << "" << std::endl;
	}
}
