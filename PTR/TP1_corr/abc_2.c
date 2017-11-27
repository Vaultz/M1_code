#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores

// compile avec
// gcc -Wall abc_2.c -lpthread

sem_t turnA;
sem_t turnB;
sem_t turnC;

void* a(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        sem_wait(&turnA);
        printf("A");
        fflush(stdout);
        sem_post(&turnB);
    }
    return NULL;
}

void* b(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        sem_wait(&turnB);
        printf("B");
        fflush(stdout);
        sem_post(&turnC);
    }
    return NULL;
}

void* c(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        sem_wait(&turnC);
        printf("C");
        fflush(stdout);
        sem_post(&turnA);
    }
    return NULL;
}

int main() {
    pthread_t ID[2];
    
    //sem_init(&turnA, 0, 0);
    sem_init(&turnA, 0, 1);
    sem_init(&turnB, 0, 0);
    sem_init(&turnC, 0, 0);
    
    pthread_create(&ID[0], NULL, b, NULL);
    pthread_create(&ID[1], NULL, c, NULL);

    //sem_post(&turnA); // si turnA initialisé à zéro
    a(NULL);

    pthread_join(ID[0], NULL);
    pthread_join(ID[1], NULL);
    
    sem_destroy(&turnA);
    sem_destroy(&turnB);
    sem_destroy(&turnC);

    return 0;
}
