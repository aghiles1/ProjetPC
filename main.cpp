#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
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




// void pause();
 
// int main(int argc, char *argv[])
// {
//     SDL_Surface *ecran = NULL;
    
//     SDL_Init(SDL_INIT_VIDEO);

//     ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
//     SDL_WM_SetCaption("Ma super fenêtre SDL !", NULL);
    
//     // Coloration de la surface ecran en bleu-vert
//     SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 17, 206, 112));

//     SDL_Flip(ecran); /* Mise à jour de l'écran avec sa nouvelle couleur */

//     pause();

//     SDL_Quit();

//     return EXIT_SUCCESS;
// }
 
// void pause()
// {
//     int continuer = 1;
//     SDL_Event event;
 
//     while (continuer)
//     {
//         SDL_WaitEvent(&event);
//         switch(event.type)
//         {
//             case SDL_QUIT:
//                 continuer = 0;
//         }
//     }
// }
