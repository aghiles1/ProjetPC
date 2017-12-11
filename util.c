#include "util.h"
#include "personne.h"
#include "define.h"
#include "case.h"
#include <math.h>

#define M_PI (3.14159265358979323846264338327950288)

void init_matrice_personnes(t_case*** matrice, int p, personne* personnes, t_portion** portions){
	srand(0);
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

		if(CASE_STATE(matrice[x][y]) == EMPTY){
			CASE_STATE(matrice[x][y]) = MEN;
			personnes[i].x = x;
			personnes[i].y = y;
			personnes[i].matrice = matrice;
			if(portions!=NULL){
				pthread_mutex_init(&personnes[i].verrou, NULL);
			}
			i++;
		}
	}
	if(portions!=NULL){
		portions[0]->matrice = matrice;
		portions[0]->personnes = personnes;
		portions[1]->matrice = matrice;
		portions[1]->personnes = personnes;
		portions[2]->matrice = matrice;
		portions[2]->personnes = personnes;
		portions[3]->matrice = matrice;
		portions[3]->personnes = personnes;
	}
}


void affiche(t_case*** m){
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			printf("%d",CASE_STATE(m[i][j]));
		}
		printf("\n");
	}
	printf("\n\n");
	fflush(stdout);
}


void* deplacer_zero(void* p){
	personne* per = (personne*)p;
	int x = per->x;
	int y = per->y;
	double dist;
	double sinA;
	while(!((x==0 && y==0) || (x==1 && y==0) || (x==0 && y==1))){
		dist = sqrt(x*x+y*y);
		sinA = ((double)(y))/dist;

		// azimuth
		if(sinA>=sin(M_PI/(8.0)) && sinA<=sin((3*M_PI)/(8.0))){
			if(x>0 && y>0) {
				pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
				if(CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					CASE_STATE(per->matrice[x-1][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
					x = --per->x;
					y = --per->y;
					continue;
				}
				pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			}
			if(sinA<sin(M_PI/(4.0))){
				if(x>0) {
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
					if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
						CASE_STATE(per->matrice[x-1][y])=MEN;
						CASE_STATE(per->matrice[x][y])=EMPTY;
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
						x = --per->x;
						continue;
					}
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
				}
				if(y>0) {
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
					if(CASE_STATE(per->matrice[x][y-1])==EMPTY){
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
						CASE_STATE(per->matrice[x][y-1])=MEN;
						CASE_STATE(per->matrice[x][y])=EMPTY;
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
						y = --per->y;
						continue;
					}
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
				}
			}
			else {
				if(y>0) {
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
					if(CASE_STATE(per->matrice[x][y-1])==EMPTY){
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
						CASE_STATE(per->matrice[x][y-1])=MEN;
						CASE_STATE(per->matrice[x][y])=EMPTY;
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
						y = --per->y;
						continue;
					}
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
				}
				if(x>0) {
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
					if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
						pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
						CASE_STATE(per->matrice[x-1][y])=MEN;
						CASE_STATE(per->matrice[x][y])=EMPTY;
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
						pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
						x = --per->x;
						continue;
					}
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
				}
			}
		}
		else if(sinA<sin(M_PI/(8.0))){
			if(x>0) {
				pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
				if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					CASE_STATE(per->matrice[x-1][y])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
					x = --per->x;
					continue;
				}
				pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			}
			if(x>0 && y>0) {
				pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
				if(CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					CASE_STATE(per->matrice[x-1][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
					x = --per->x;
					y = --per->y;
					continue;
				}
				pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			}
			if(y>0) {
				pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
				if(CASE_STATE(per->matrice[x][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					CASE_STATE(per->matrice[x][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
					y = --per->y;
					continue;
				}
				pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			}
		}
		else{
			if(y>0) {
				pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
				if(CASE_STATE(per->matrice[x][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					CASE_STATE(per->matrice[x][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
					y = --per->y;
					continue;
				}
				pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			}
			if(x>0 && y>0) {
				pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
				if(CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					CASE_STATE(per->matrice[x-1][y-1])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
					x = --per->x;
					y = --per->y;
					continue;
				}
				pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			}
			if(x>0) {
				pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
				if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
					pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
					CASE_STATE(per->matrice[x-1][y])=MEN;
					CASE_STATE(per->matrice[x][y])=EMPTY;
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
					pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
					x = --per->x;
					continue;
				}
				pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			}
		}
	}
	pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
	if(CASE_STATE(per->matrice[per->x][per->y]) == MEN)
		CASE_STATE(per->matrice[per->x][per->y])=EMPTY;
	pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));

	pthread_exit(NULL);
}

// NE PAS TOUCHER CETTE VARIABLE GLOBALE.....
// UTILISEE DANS create_thread_portion et deplacer_un
int count = 0;

void* deplacer_un(void* p){
	t_portion* por = (t_portion*)p;

	personne* per;
	int current_per = 0;
	int x;
	int y;
	double dist;
	double sinA;

	while(1) {
		if(count == por->nb_personnes){
			break;
		}

		pthread_mutex_lock(&por->personnes[current_per].verrou);

		per = &por->personnes[current_per];
		current_per++;
		if(current_per==por->nb_personnes)
			current_per = 0;

		if(per->x >= por->xmin && per->x <= por->xmax && per->y >= por->ymin && per->y <= por->ymax) {
			x = per->x;
			y = per->y;

			if((x==0 && y==0) || (x==1 && y==0) || (x==0 && y==1)){
				CASE_STATE(per->matrice[per->x][per->y])=EMPTY;
				per->x = -1;
				per->y = -1;
				count++;
				pthread_mutex_unlock(&per->verrou);
				continue;
			}

			dist = sqrt(x*x+y*y);
			sinA = ((double)(y))/dist;

			// azimuth
			if(sinA>=sin(M_PI/(8.0)) && sinA<=sin((3*M_PI)/(8.0))){
			  if(x>0 && y>0) {
			    if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
			    if(CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
			      if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			      CASE_STATE(per->matrice[x-1][y-1])=MEN;
			      CASE_STATE(per->matrice[x][y])=EMPTY;
			      if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			      if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			      x = --per->x;
			      y = --per->y;
			      pthread_mutex_unlock(&per->verrou);
			      continue;
			    }
			    if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			  }
			  if(sinA<sin(M_PI/(4.0))){
			    if(x>0) {
			      if(per->x-1 == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
			      if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
			        if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			        CASE_STATE(per->matrice[x-1][y])=MEN;
			        CASE_STATE(per->matrice[x][y])=EMPTY;
			        if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			        if(per->x-1 == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			        x = --per->x;
			        pthread_mutex_unlock(&per->verrou);
			        continue;
			      }
			      if(per->x-1 == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			    }
			    if(y>0) {
			      if(per->x == 63 || per->y-1 == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
			      if(CASE_STATE(per->matrice[x][y-1])==EMPTY){
			        if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			        CASE_STATE(per->matrice[x][y-1])=MEN;
			        CASE_STATE(per->matrice[x][y])=EMPTY;
			        if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			        if(per->x == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			        y = --per->y;
			        pthread_mutex_unlock(&per->verrou);
			        continue;
			      }
			      if(per->x == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			    }
			  }
			  else {
			    if(y>0) {
			      if(per->x == 63 || per->y-1 == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
			      if(CASE_STATE(per->matrice[x][y-1])==EMPTY){
			        if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			        CASE_STATE(per->matrice[x][y-1])=MEN;
			        CASE_STATE(per->matrice[x][y])=EMPTY;
			        if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			        if(per->x == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			        y = --per->y;
			        pthread_mutex_unlock(&per->verrou);
			        continue;
			      }
			      if(per->x == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			    }
			    if(x>0) {
			      if(per->x-1 == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
			      if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
			        if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			        CASE_STATE(per->matrice[x-1][y])=MEN;
			        CASE_STATE(per->matrice[x][y])=EMPTY;
			        if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			        if(per->x-1 == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			        x = --per->x;
			        pthread_mutex_unlock(&per->verrou);
			        continue;
			      }
			      if(per->x-1 == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			    }
			  }
			}
			else if(sinA<sin(M_PI/(8.0))){
			  if(x>0) {
			    if(per->x-1 == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
			    if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
			      if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			      CASE_STATE(per->matrice[x-1][y])=MEN;
			      CASE_STATE(per->matrice[x][y])=EMPTY;
			      if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			      if(per->x-1 == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			      x = --per->x;
			      pthread_mutex_unlock(&per->verrou);
			      continue;
			    }
			    if(per->x-1 == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			  }
			  if(x>0 && y>0) {
			    if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
			    if(CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
			      if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			      CASE_STATE(per->matrice[x-1][y-1])=MEN;
			      CASE_STATE(per->matrice[x][y])=EMPTY;
			      if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			      if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			      x = --per->x;
			      y = --per->y;
			      pthread_mutex_unlock(&per->verrou);
			      continue;
			    }
			    if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			  }
			  if(y>0) {
			    if(per->x == 63 || per->y-1 == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
			    if(CASE_STATE(per->matrice[x][y-1])==EMPTY){
			      if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			      CASE_STATE(per->matrice[x][y-1])=MEN;
			      CASE_STATE(per->matrice[x][y])=EMPTY;
			      if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			      if(per->x == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			      y = --per->y;
			      pthread_mutex_unlock(&per->verrou);
			      continue;
			    }
			    if(per->x == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			  }
			}
			else{
			  if(y>0) {
			    if(per->x == 63 || per->y-1 == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y-1]));
			    if(CASE_STATE(per->matrice[x][y-1])==EMPTY){
			      if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			      CASE_STATE(per->matrice[x][y-1])=MEN;
			      CASE_STATE(per->matrice[x][y])=EMPTY;
			      if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			      if(per->x == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			      y = --per->y;
			      pthread_mutex_unlock(&per->verrou);
			      continue;
			    }
			    if(per->x == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y-1]));
			  }
			  if(x>0 && y>0) {
			    if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y-1]));
			    if(CASE_STATE(per->matrice[x-1][y-1])==EMPTY){
			      if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			      CASE_STATE(per->matrice[x-1][y-1])=MEN;
			      CASE_STATE(per->matrice[x][y])=EMPTY;
			      if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			      if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			      x = --per->x;
			      y = --per->y;
			      pthread_mutex_unlock(&per->verrou);
			      continue;
			    }
			    if(per->x-1 == 63 || per->y-1 == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y-1]));
			  }
			  if(x>0) {
			    if(per->x-1 == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x-1][y]));
			    if(x>0 && CASE_STATE(per->matrice[x-1][y])==EMPTY){
			      if(per->x == 63 || per->y == 255) pthread_mutex_lock(&CASE_VERROU(per->matrice[x][y]));
			      CASE_STATE(per->matrice[x-1][y])=MEN;
			      CASE_STATE(per->matrice[x][y])=EMPTY;
			      if(per->x == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x][y]));
			      if(per->x-1 == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			      x = --per->x;
			      pthread_mutex_unlock(&per->verrou);
			      continue;
			    }
			    if(per->x-1 == 63 || per->y == 255) pthread_mutex_unlock(&CASE_VERROU(per->matrice[x-1][y]));
			  }
			}
		}
		pthread_mutex_unlock(&per->verrou);
	}

	pthread_exit(NULL);
}

void create_threads_personnes(pthread_t* tab, int nb, personne* personnes){
	//création des threads pour chaque personne
	for(int i=0; i<nb; i++)
		pthread_create(&(tab[i]), NULL, deplacer_zero, (void*)&(personnes[i]));
}

void create_threads_portions(pthread_t* tab, t_portion** portions){
	//création des threads pour chaque personne
	count = 0;
	for(int i=0; i<4; i++) {
		pthread_create(&(tab[i]), NULL, deplacer_un, (void*)(portions[i]));
	}
}
