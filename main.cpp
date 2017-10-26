#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#include "define.h"
#include "matrice.h"
#include <time.h>

#include "sys/times.h"
#include "sys/vtimes.h"

#define T0_MOD false
#define T1_MOD true


void initCPU();
double getCurrentValue();

int main(int argc, char** argv) {

	int nbp;
	bool mode, mesure = false;
	clock_t t1,t2;
	//vérification des arguments
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
		//nombre de personnes
		int nb = (int)pow(2, nbp);
		//récupérer un tableau de personnes
		Per* personnes = init(matrice_jeu,nb);
		//récupérer un tableau de PID de threads
		pthread_t* tid = create_threads_personnes(personnes,nb);
		//attendre la fin des thread avant que le programme s'arrete
		for (int i = 0; i < nb; i++)
			pthread_join(tid[i], NULL);
		//afficher la matrice
	   	//affiche(matrice_jeu,HEIGHT,WIDTH);
	   	//suppression de la memoire allouée pour ne pas avoir de fuite de memoire
		delete[] personnes;
		delete[] matrice_jeu;
	}
	else if(mode == T1_MOD){
		std::cout << "Fonction non implémentée..." << std::endl;
	}
/*
		//AVANT
	initCPU();
	t1=clock();
		//PENDANT
	
    t2=clock();
    double temps = (((double)t2)-((double)t1))/((double)CLOCKS_PER_SEC);
    std::cout << "time: "  << (int)temps  << " Sec et " << (temps-(int)temps)*1000 << " MiliSec" << "  CPU " << getCurrentValue() << "%" << std::endl;*/
   	

	return 0;
}



    static clock_t lastCPU, lastSysCPU, lastUserCPU;
    static int numProcessors;

    void initCPU(){
        FILE* file;
        struct tms timeSample;
        char line[128];

        lastCPU = times(&timeSample);
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;

        file = fopen("/proc/cpuinfo", "r");
        numProcessors = 0;
        while(fgets(line, 128, file) != NULL){
            if (strncmp(line, "processor", 9) == 0) numProcessors++;
        }
        fclose(file);
    }

    double getCurrentValue(){
        struct tms timeSample;
        clock_t now;
        double percent;

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
        lastCPU = now;
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;

        return percent;
    }
