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
#include "personne.h"
#include <time.h>

#include <vector>

#include "sys/times.h"
#include "sys/vtimes.h"
#include <sys/resource.h>

void initCPU();
long double getCurrentValue();

#define UNDEFINED_MOD -1
#define T0_MOD 0
#define T1_MOD 1

int main(int argc, char** argv) {

	int nbp = -1, mode = UNDEFINED_MOD;
	int mesure = false;
	struct tms time1,time2;
	//vérification des arguments
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-p")==0){
			i++;
			if(i >= argc)
				break;
			nbp = atoi(argv[i]);
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
	if (nbp < 0){
		std::cerr << "Argument -p suivi de [0-9] non présent ou en dehors de la plage de données acceptée." << std::endl;
		exit(1);
	}
	if (mode == UNDEFINED_MOD){
		std::cerr << "Aucun mode choisi ! (-t[01]).s" << std::endl;
		exit(1);
	}

	pthread_t* tid = static_cast<pthread_t*> (malloc(sizeof(pthread_t)*(int)pow(2, nbp)));
	std::vector<Personne>* personnes = new std::vector<Personne>();

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
		double tab_tempsRep[5] = {};

		int nb = (static_cast<int>(pow(2, nbp)));
		int it = (mesure) ? 5 : 1;
		for(int exec = 0; exec < it; exec++){
		//récupérer un tableau de personnes
			init(matrice_jeu, nb, personnes);
			//récupérer un tableau de PID de threads
			initCPU();
			times(&time1);
			create_threads_personnes(tid, nb, personnes);
			//attendre la fin des thread avant que le programme s'arrete
			for (int i = 0; i < nb; i++){
		       		if(pthread_join(tid[i], NULL) != 0)
					printf("bug_join\n");
			};
			times(&time2);
			int tics_per_second;
			tics_per_second = sysconf(_SC_CLK_TCK);
		    tab_mesures[exec] = getCurrentValue();
		    tab_temps[exec] = (((double)time2.tms_utime)-((double)time1.tms_utime))/((double)tics_per_second);
		    tab_tempsRep[exec] = (((double)time2.tms_stime+time2.tms_utime)-((double)time1.tms_stime+time1.tms_utime))/((double)tics_per_second);
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
						double tmp3 = tab_tempsRep[z];
						tab_tempsRep[z] = tab_tempsRep[z+1];
						tab_tempsRep[z+1] = tmp3;
					}
				}
				taille--;
			}
			std::cout << "MESURES MOYENNES : \n\t temps CPU consommé: " << (tab_temps[1]+tab_temps[2]+tab_temps[3])/((long)3)*1000 << " MiliSec  \n\t temps de réponse: "<< (tab_tempsRep[1]+tab_tempsRep[2]+tab_tempsRep[3])/((long)3)*1000 <<  " MiliSec \n\t" << " CPU " << (tab_mesures[1]+tab_mesures[2]+tab_mesures[3])/((long double)3) << "%" << std::endl;
		}
	}
	else if(mode == T1_MOD){
		std::cout << "Fonction non implémentée..." << std::endl;
	}

	return 0;
}

    clock_t lastCPU, lastSysCPU, lastUserCPU;
    int numProcessors;
	/**
	 * [initCPU description]
	 */
    void initCPU(){
        struct tms timeSample;
        char line[128];

        lastCPU = times(&timeSample);
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;
        numProcessors = sysconf(_SC_NPROCESSORS_ONLN);
    }
	/**
	 * [getCurrentValue description]
	 * @return [description]
	 */
    long double getCurrentValue(){
        struct tms timeSample;
        clock_t now;
        long double percent;

        now = times(&timeSample);
        if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
            timeSample.tms_utime < lastUserCPU){
            //Overflow detection. Just skip this value.
            percent = 0.0;
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
