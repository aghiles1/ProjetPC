#include "thread_personne.h"

/*
 for (i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
*/

void* rien(){
	return NULL;
}

pthread_t* create_threads_personnes(int nb){

	pthread_t* personnes = malloc(sizeof(pthread_t)*nb);
	if(personnes == NULL) 
		std::cerr << "ERREUR CREATION THREADS" << std::endl;

	for(int i=0; i<nb; i++)
		pthread_create(&personnes[i], NULL, rien, NULL);
//join
	return personnes;
}


/*
#include <sys/syscall.h>
#include <sys/types.h>

void *affich(void *arg){
    int n = *(int*) arg;

    for(int i=0; i<20; i++){
        printf("%d\n", getpid());
        printf("%d\n", (int)pthread_self());
        printf("Linux Thread ID: %ld\n", syscall(SYS_gettid));

        sleep(n);
    }

    return NULL;
}

int main(int argc, char** argv){
    if(argc != 3){
        printf("2 arguments entiers sont attendus");
        exit(1);
    }

    int n[2] = {atoi(argv[1]),atoi(argv[2])};

    int i;
    pthread_t threads[2];

    /* Creation des threads 
    for (i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, affich, (void *)&n[i]);
    }

    /* On attend que ça se termine 
    for (i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
*/
