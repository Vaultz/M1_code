#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> // struct timespec

// Discussion : ecritures concurentes
// $ gcc -Wall ecritures-concurentes.c -lpthread

void traitementLong() {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 10;
    nanosleep(&ts, NULL); // on simule un traitement long
    // note : usleep behavior inconsistent when it is interrupted
    //        by a signal among historical systems
    //        nanosleep returns immediately when any signal handler is executed
}

int VALEUR;

void* f0(void* _p) {
    int i;
    for(i = 0; i < 1000; i++) {
        int x = VALEUR;
        traitementLong(); // VALEUR est modifiée par l'autre thread pendant ce temps...
        VALEUR = x + 1;
    }
    puts("f0_fini");
    return NULL;
}

void* f1(void* _p) {
    int i;
    for(i = 0; i < 1000; i++) {
        int x = VALEUR;
        traitementLong();
        VALEUR = x + 2;
    }
    puts("f1_fini");
    return NULL;
}

int main() {
    pthread_t pid[2];

    pthread_create(&pid[0], NULL, f0, NULL); // créer un thread
    pthread_create(&pid[1], NULL, f1, NULL); // pid, attributs, fonction, argument

    pthread_join(pid[0], 0); // attendre la fin d'un thread
    pthread_join(pid[1], 0);
    
    printf("VALEUR=%d\n", VALEUR);
    
    return 0;
}
