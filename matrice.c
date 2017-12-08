#include "matrice.h"
#include "personne.h"
#include "define.h"
#include "case.h"
#include <math.h>

#define M_PI (3.14159265358979323846264338327950288)

void init_matrice_personnes(t_case*** matrice, int p, personne* personnes){

		for (int i = 40, i1 = 80, i2=10, i3=50, i4=35; i <= 60 || i1 <= 110 || i2<=30 || i3<=90 || i4<=45 ; i++,i1++,i2++,i3++,i4++)
		{
			for (int j = 20, j1= 85, j2=180, j3=220,j4=300; j <= 60 || j1 <= 90 || j2<=190 || j3<=250 || j4<=450; j++,j1++,j2++,j3++,j4++)
			{

				if(i <= 60 && j <= 60){
					CASE_STATE(matrice[i][j]) = WALL;
				}
				if(i1 <= 110 && j1 <= 90){
					CASE_STATE(matrice[i1][j1]) = WALL;
				}
				if(i2 <= 30 && j2 <= 190){
					CASE_STATE(matrice[i2][j2]) = WALL;
				}
				if(i3 <= 90 && j3 <= 250){
					CASE_STATE(matrice[i3][j3]) = WALL;
				}
				if(i4 <= 45 && j4 <= 450){
					CASE_STATE(matrice[i4][j4]) = WALL;
				}
			}

		}
		int i=0;
		while (i < p)
		{
			int x = rand() % (HEIGHT), y = rand() % (WIDTH);

			//std::cout << "pos = "<<x<<" x== "<<personnes[x]->x  <<" y== "<<personnes[x]->y << std::endl;
			if(CASE_STATE(matrice[x][y]) == EMPTY){
				CASE_STATE(matrice[x][y]) = MEN;
				personnes[i].x = x;
				personnes[i].y = y;
				personnes[i].matrice = matrice;
				i++;
			}
		}
}


void* deplacer(void* p){
	personne* per = (personne*)p;
	    int x =per->x;
	    int y =per->y;
		double dist;
		double sinA;
	    while(!((x==0 && y==0) || (x==1 && y==0) || (x==0 && y==1))){
	         x =per->x;
	         y =per->y;

		dist = sqrt(x+y);
		sinA = ((double)(y))/dist;

	        // azimuth
			if(sinA>=sin(M_PI/(8.0)) && sinA<=sin((3*M_PI)/(8.0))){
				if(x>0 && y>0 && CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
					CASE_STATE(per->matrice[x-1][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
					per->y--;
					per->x--;
				}
				else if(sinA<sin(M_PI/(4.0))){
					if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
						CASE_STATE(per->matrice[x-1][y])=MEN;
						CASE_STATE(per->matrice[x][y])=EMPTY;
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
						per->x--;

					}
					else if(y>0 && CASE_STATE(per->matrice[x][y-1])==EMPTY){
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
						CASE_STATE(per->matrice[x][y-1])=MEN;
						CASE_STATE(per->matrice[x][y])=EMPTY;
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
						per->y--;
					}
				}
				else {
					if(y>0 && CASE_STATE(per->matrice[x][y-1])==EMPTY){
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
						CASE_STATE(per->matrice[x][y-1])=MEN;
						CASE_STATE(per->matrice[x][y])=EMPTY;
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
						per->y--;
					}
					else if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
						CASE_STATE(per->matrice[x-1][y])=MEN;
						CASE_STATE(per->matrice[x][y])=EMPTY;
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
						per->x--;

					}
				}
			}
			else if(sinA<sin(M_PI/(8.0))){
				if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
					CASE_STATE(per->matrice[x-1][y])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
					per->x--;

				}
				else if(x>0 && y>0 && CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
					CASE_STATE(per->matrice[x-1][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
					per->y--;
					per->x--;
				}
				else if(y>0 && CASE_STATE(per->matrice[x][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
					CASE_STATE(per->matrice[x][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
					per->y--;
				}
			}
			else{
				if(y>0 && CASE_STATE(per->matrice[x][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
					CASE_STATE(per->matrice[x][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
					per->y--;
				}
				else if(x>0 && y>0 && CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
					CASE_STATE(per->matrice[x-1][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
					per->y--;
					per->x--;
				}
				else if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
					CASE_STATE(per->matrice[x-1][y])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
					per->x--;

				}
			}
	    }
			pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
	    if(CASE_STATE(per->matrice[per->x][per->y]) == MEN)
	    	CASE_STATE(per->matrice[per->x][per->y])=EMPTY;
				pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));

	return NULL;
}

void create_threads_personnes(pthread_t* tab, int nb, personne* personnes){
    //création des threads pour chaque personne
	for(int i=0; i<nb; i++)
		pthread_create(&(tab[i]), NULL, deplacer, (void*)&(personnes[i]));
}
