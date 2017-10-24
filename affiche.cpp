#include "affiche.h"
#include <iostream>


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