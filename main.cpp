#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "init.h"
#include "affiche.h"
#include <iostream>


int main() {

	int** matrice_jeu = init(HEIGHT,WIDTH,9);
	affiche(matrice_jeu,HEIGHT,WIDTH);
	return 0;

}

