#include "matrice.h"
#include "personne.h"
#include "define.h"
#include <math.h>
#include <cstdlib>

pthread_mutex_t lock;
int** matrice_jeu1;

void init(int** matrice_jeu, int p, std::vector<Personne>* personnes){

		for (int i = 40, i1 = 80, i2=10, i3=50, i4=35; i <= 60 || i1 <= 110 || i2<=30 || i3<=90 || i4<=45 ; i++,i1++,i2++,i3++,i4++)
		{
			for (int j = 20, j1= 85, j2=180, j3=220,j4=300; j <= 60 || j1 <= 90 || j2<=190 || j3<=250 || j4<=450; j++,j1++,j2++,j3++,j4++)
			{

				if(i <= 60 && j <= 60){
					*(*(matrice_jeu+i)+j) = WALL;
				}
				if(i1 <= 110 && j1 <= 90){
					*(*(matrice_jeu+i1)+j1) = WALL;
				}
				if(i2 <= 30 && j2 <= 190){
					*(*(matrice_jeu+i2)+j2) = WALL;
				}
				if(i3 <= 90 && j3 <= 250){
					*(*(matrice_jeu+i3)+j3) = WALL;
				}
				if(i4 <= 45 && j4 <= 450){
					*(*(matrice_jeu+i4)+j4) = WALL;
				}
			}

		}
		int i=0;
		while (i < p)
		{
			int x = rand() % (HEIGHT), y = rand() % (WIDTH);

			//std::cout << "pos = "<<x<<" x== "<<personnes[x]->x  <<" y== "<<personnes[x]->y << std::endl;
			if(matrice_jeu[x][y] == EMPTY){
				matrice_jeu[x][y] = MEN;
				personnes->push_back(Personne(x, y));
				i++;
			}
		}
		matrice_jeu1 =matrice_jeu;
}

void affiche(int** matrice_jeu,int hight,int width){
	system("clear");
	for (int i = 0; i < hight-1; ++i){
		for (int j = 0; j < width-1; ++j){
			std::cout << matrice_jeu[i][j];
			fflush(stdin);
		}
		std::cout << "" << std::endl;
	}
	std::cout << "" << std::endl;
}

void *deplacer(void* p){
	Personne& personne = *(static_cast<Personne*> (p));
    int x =personne.x;
    int y =personne.y;
	double dist;
	double sinA;
    while(!((x==0 && y==0) || (x==1 && y==0) || (x==0 && y==1))){
         x =personne.x;
         y =personne.y;

	dist = sqrt(x+y);
	sinA = (static_cast<double>(y))/dist;

         //verrouiller la section critique pour ne pas ecraser des valeurs
        pthread_mutex_lock(&lock);
        // azimuth
		if(sinA>=sin(M_PI/(8.0)) && sinA<=sin((3*M_PI)/(8.0))){
			if(x>0 && y>0 && matrice_jeu1[x-1][y-1]==EMPTY){
				matrice_jeu1[x-1][y-1]=MEN;
				matrice_jeu1[x][y]=EMPTY;
				personne.y--;
				personne.x--;
			}
			else if(sinA<sin(M_PI/(4.0))){
				if(x>0 && matrice_jeu1[x-1][y]==EMPTY){
					matrice_jeu1[x-1][y]=MEN;
					matrice_jeu1[x][y]=EMPTY;
					personne.x--;

				}
				else if(y>0 && matrice_jeu1[x][y-1]==EMPTY){
					matrice_jeu1[x][y-1]=MEN;
					matrice_jeu1[x][y]=EMPTY;
					personne.y--;
				}
			}
			else {
				if(y>0 && matrice_jeu1[x][y-1]==EMPTY){
					matrice_jeu1[x][y-1]=MEN;
					matrice_jeu1[x][y]=EMPTY;
					personne.y--;
				}
				else if(x>0 && matrice_jeu1[x-1][y]==EMPTY){
					matrice_jeu1[x-1][y]=MEN;
					matrice_jeu1[x][y]=EMPTY;
					personne.x--;
				}
			}
		}
		else if(sinA<sin(M_PI/(8.0))){
			if(x>0 && matrice_jeu1[x-1][y]==EMPTY){
				matrice_jeu1[x-1][y]=MEN;
				matrice_jeu1[x][y]=EMPTY;
				personne.x--;

			}
			else if(x>0 && y>0 && matrice_jeu1[x-1][y-1]==EMPTY){
				matrice_jeu1[x-1][y-1]=MEN;
				matrice_jeu1[x][y]=EMPTY;
				personne.y--;
				personne.x--;
			}
			else if(y>0 && matrice_jeu1[x][y-1]==EMPTY){
				matrice_jeu1[x][y-1]=MEN;
				matrice_jeu1[x][y]=EMPTY;
				personne.y--;
			}
		}
		else{
			if(y>0 && matrice_jeu1[x][y-1]==EMPTY){
				matrice_jeu1[x][y-1]=MEN;
				matrice_jeu1[x][y]=EMPTY;
				personne.y--;


			}
			else if(x>0 && y>0 && matrice_jeu1[x-1][y-1]==EMPTY){
				matrice_jeu1[x-1][y-1]=MEN;
				matrice_jeu1[x][y]=EMPTY;
				personne.y--;
				personne.x--;
			}
			else if(x>0 && matrice_jeu1[x-1][y]==EMPTY){
				matrice_jeu1[x-1][y]=MEN;
				matrice_jeu1[x][y]=EMPTY;
				personne.x--;
			}
		}
        	//on déverrouille le mutex pour que les autres threads puissent deplacer les autres personnes
        pthread_mutex_unlock(&lock);

    }
    if(matrice_jeu1[personne.x][personne.y] == MEN)
    	matrice_jeu1[personne.x][personne.y]=EMPTY;//la personne sort de la matrice
// pthread_mutex_lock(&lock);
// affiche(matrice_jeu1,128,512);
// pthread_mutex_unlock(&lock);
    return NULL;
}


void create_threads_personnes(pthread_t* tab, int nb, std::vector<Personne>* personnes){
	//création d'un tableau pour les pid des threads
	if(tab == NULL){
		std::cerr << "ERREUR CREATION THREADS" << std::endl;
		exit(1);
	}
	//initialiser le mutex
    if (pthread_mutex_init(&lock, NULL) != 0){
        std::cerr << "ERREUR INITIALISATION MUTEX" << std::endl;
        exit(1);
    }
    //création des threads pour chaque personne
	for(int i=0; i<nb; i++){
		int p= pthread_create(&tab[i], NULL, deplacer, &(personnes->at(i)));
		if(p == EPERM || p== EAGAIN || p==EINVAL)
			printf("bug_create\n");
	}

}
