#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

#include "define.h"
#include "matrice.h"
#include <time.h>

#include "sys/times.h"
#include "sys/vtimes.h"
#include <sys/resource.h>

void initCPU();
long double getCurrentValue();

#define T0_MOD false
#define T1_MOD true

int main(int argc, char** argv) {

	int nbp;
	bool mode, mesure = false;
	clock_t t1,t2;
	//vérification des arguments
	if (argc == 1)
	{
		/* prteger la ligne d'argument fusion des mesures  */
		//TODO
	}
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-p")==0){
			nbp = atoi(argv[++i]);
			if(nbp > 9 || nbp < 0){
				std::cerr << "ERREUR PARAMETRE -p [0-9] EN DEHORS DE LA PLAGE" << std::endl;
				exit(1);
			}
			continue;
		}
		if(strcmp(argv[i], "-t0")==0){
			mode = T0_MOD;
			continue;
		}
		if(strcmp(argv[i], "-t1")==0){
			mode = T1_MOD;
			continue;
		}
		if(strcmp(argv[i], "-m")==0){
			mesure = true;
			continue;
		}
	}

	pthread_t* tid = (pthread_t*) malloc(sizeof(pthread_t)*(int)pow(2, nbp));
	Per* personnes = static_cast<Per*> (malloc(sizeof(Per)*(int)pow(2, nbp)));

	if(mode == T0_MOD){
		//création de la matrice du jeu (lignes)
		int** matrice_jeu = static_cast<int**> (malloc(sizeof(int*)*HEIGHT));
		if(matrice_jeu == NULL){
			std::cerr << "ERREUR DE GENERATION DE LA MATRICE" << std::endl;
			exit(1);
		}
		//création de la matrice du jeu (colonnes)
		for (int i = 0; i < HEIGHT; i++){
			*(matrice_jeu+i) = static_cast<int*> (malloc(sizeof(int)*WIDTH));
			if(*(matrice_jeu+i) == NULL){
				std::cerr << "ERREUR DE GENERATION DE LA MATRICE" << std::endl;
				exit(1);
			}
		}
		long double tab_mesures[5] = {};
		double tab_temps[5] = {};

		int nb = (int)pow(2, nbp);
		int it = (mesure) ? 5 : 1;
		for(int exec = 0; exec < it; exec++){
		//récupérer un tableau de personnes
			init(matrice_jeu,nb,personnes);
			//récupérer un tableau de PID de threads
			initCPU();
			t1=clock();
			create_threads_personnes(personnes,nb,tid);
			//attendre la fin des thread avant que le programme s'arrete
			for (int i = 0; i < nb; i++){
		       		if(pthread_join(tid[i], NULL) != 0)
					printf("bug_join\n");
			};
			t2=clock();
		    tab_mesures[exec] = getCurrentValue();
		    tab_temps[exec] = (((double)t2)-((double)t1))/((double)CLOCKS_PER_SEC);
		}
		if(mesure){
			bool trie = false;
			int taille = 5;
			while(!trie){
				trie = true;
				for(int z=0 ; z < taille-1 ; z++){
					if(tab_temps[z] > tab_temps[z+1]){
						double tmp = tab_temps[z];
						tab_temps[z] = tab_temps[z+1];
						tab_temps[z+1] = tmp;
						long double tmp2 = tab_mesures[z];
						tab_mesures[z] = tab_mesures[z+1];
						tab_mesures[z+1] = tmp2;
					}
				}
				taille--;
			}
			std::cout << "MESURES MOYENNES = time: " << (tab_temps[1]+tab_temps[2]+tab_temps[3])/((long)3)*1000 << " MiliSec" << "  CPU " << (tab_mesures[1]+tab_mesures[2]+tab_mesures[3])/((long double)3) << "%" << std::endl;
		}
	}
	else if(mode == T1_MOD){
		std::cout << "Fonction non implémentée..." << std::endl;
	}

	return 0;
}

    clock_t lastCPU, lastSysCPU, lastUserCPU;
    int numProcessors;

    void initCPU(){
        struct tms timeSample;
        char line[128];

        lastCPU = times(&timeSample);
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;
        numProcessors = sysconf(_SC_NPROCESSORS_ONLN);
    }

    long double getCurrentValue(){
        struct tms timeSample;
        clock_t now;
        long double percent;

        now = times(&timeSample);
        if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
            timeSample.tms_utime < lastUserCPU){
            //Overflow detection. Just skip this value.
            percent = -1.0;
        }
        else{
            percent = (timeSample.tms_stime - lastSysCPU) +
                (timeSample.tms_utime - lastUserCPU);
            percent /= (now - lastCPU);
            percent /= numProcessors;
            percent *= 100;
        }
        

        return percent;
    }
