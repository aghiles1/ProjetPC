#include "matrice.h"
#include "define.h"
#include <cstdlib>
pthread_mutex_t lock;
int** matrice_jeu1;

Per* init(int** matrice_jeu,int p){
		
		Per* personnes = static_cast<Per*> (malloc(sizeof(Per)*p));
		
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
		int x=0;
		while (x < p)
		{	if (personnes[x] == nullptr)
			{
				personnes[x] = new Personne();
			}
			personnes[x]->x = rand() % (HEIGHT);
			personnes[x]->y = rand() % (WIDTH);
			std::cout << "pos = "<<x<<" x== "<<personnes[x]->x  <<" y== "<<personnes[x]->y << std::endl;	
			if(*(*(matrice_jeu+personnes[x]->x)+personnes[x]->y) == EMPTY){
				*(*(matrice_jeu+personnes[x]->x)+personnes[x]->y) = MEN;
				x++;
			}	
			
		}
		matrice_jeu1 =matrice_jeu; 

	return personnes;
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

void deplacer(void* p){
	Per* personne = (Per*) p;
    int x =(*personne)->x;
    int y =(*personne)->y;
    while(!((x==0 && y==0) || (x==1 && y==0) || (x==0 && y==1))){
         x =(*personne)->x;
         y =(*personne)->y;
         //verrouiller la section critique pour ne pas ecraser des valeurs
        pthread_mutex_lock(&lock);
        //tant que on peut se déplacer a la diagonal on le fait car c'est le plus rentable
        	if(x>0 && y>0 && matrice_jeu1[x-1][y-1]==EMPTY){
        		matrice_jeu1[x-1][y-1]=MEN;
        		matrice_jeu1[x][y]=EMPTY;
        		(*personne)->y--; 
        		(*personne)->x--; 
        	}
        	else //si x et y sont plus grand que 0 on avance horizontalement vers la sortie
        		if(x>0 && y>0 && matrice_jeu1[x][y-1]==EMPTY){
        		matrice_jeu1[x][y-1]=MEN;
        		matrice_jeu1[x][y]=EMPTY;
        		(*personne)->y--;  
        	}
        	else //si  si x et y sont plus grand que 0 on avance verticalement vers la sortie
        		if(x>0 && y>0 && matrice_jeu1[x-1][y]==EMPTY){
        		matrice_jeu1[x-1][y]=MEN;
        		matrice_jeu1[x][y]=EMPTY;
        		(*personne)->x--; 
        	}else //si y == 0 on avance verticalement vers la sortie
        		if(x>0 && matrice_jeu1[x-1][y]==EMPTY){
        		matrice_jeu1[x-1][y]=MEN;
        		matrice_jeu1[x][y]=EMPTY;
        		(*personne)->x--; 
        	}else //si y == 0 on avance horizontalement vers la sortie
        	 if (y>0 && matrice_jeu1[x][y-1]==EMPTY)
        	{
        		matrice_jeu1[x][y-1]=MEN;
        		matrice_jeu1[x][y]=EMPTY;
        		(*personne)->y--; 
        	}
        	//affiche( matrice_jeu1, HEIGHT,WIDTH);
        	//on déverrouille le mutex pour que les autres threads puissent deplacer les autres personnes
        pthread_mutex_unlock(&lock);

    }
    if(matrice_jeu1[y =(*personne)->x][y =(*personne)->y]=MEN)
    	matrice_jeu1[x =(*personne)->x][y =(*personne)->y]=EMPTY;//la personne sort de la matrice
}



pthread_t* create_threads_personnes(Per* tab,int nb){
	//création d'un tableau pour les pid des threads
	pthread_t* personnes = (pthread_t*) malloc(sizeof(pthread_t)*nb);
	if(personnes == NULL) 
		std::cerr << "ERREUR CREATION THREADS" << std::endl;
	//initialiser le mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        std::cerr<<"\n mutex init failed\n"<<std::endl;
        exit(1);
    }
    //création des threads pour chaque personne
	for(int i=0; i<nb; i++)
		pthread_create(&personnes[i], NULL, deplacer, &(tab[i]));
	return personnes;
}
