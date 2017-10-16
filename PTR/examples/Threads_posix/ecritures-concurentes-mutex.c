#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> // struct timespec

// Discussion : ecritures concurentes
// $ gcc -Wall ecritures-concurentes.c -lpthread

int VALEUR;
pthread_mutex_t mutex; // mutex pour protéger VALEUR

void traitementLong() {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 10;
    nanosleep(&ts, NULL); // on simule un traitement long
    // note : usleep behavior inconsistent when it is interrupted
    //        by a signal among historical systems
    //        nanosleep returns immediately when any signal handler is executed
}

void* f0(void* _p) {
    int i;
    for(i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex); //---- vérouillage
        int x = VALEUR;
        traitementLong();  // on ajoute un traitement long
        VALEUR = x + 1; // VALEUR n'est pas modifiée par l'autre thread pendant ce temps...
        pthread_mutex_unlock(&mutex); //---- dé-vérouillage
    }
    puts("f0_fini");
    return NULL;
}

void* f1(void* _p) {
    int i;
    for(i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex); //---- vérouillage - attend tant qu'il est vérouillé par f0
        int x = VALEUR;
        traitementLong();  // on ajoute un traitement long
        VALEUR = x + 2;
        pthread_mutex_unlock(&mutex); //---- dé-vérouillage
    }
    puts("f1_fini");
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL); //----  Initialiser le mutex !! Important !
    pthread_t pid[2];

    pthread_create(&pid[0], NULL, f0, NULL); // créer un thread
    pthread_create(&pid[1], NULL, f1, NULL); // pid, attributs, fonction, argument

    // les threads travaillent

    pthread_join(pid[0], 0); // attendre la fin d'un thread
    pthread_join(pid[1], 0);
    
    pthread_mutex_destroy(&mutex); //----  Détruire le mutex
    printf("VALEUR=%d\n", VALEUR);
    
    return 0;
}
