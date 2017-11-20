#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...

sem_t * turnA;
sem_t * turnB;
sem_t * turnC;
sem_t * turnD;

void* a(void* p) {
    int i;
    for(i = 0; i < 300; i++) {
        sem_wait(turnA);
        printf("A");
        fflush(stdout);
        sem_post(turnB);
    }
    return NULL;
}

void* b(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        sem_wait(turnB);
        sem_wait(turnB);
        sem_wait(turnB);
        printf("B");
        fflush(stdout);
        sem_post(turnC);
        sem_post(turnC);
    }
    return NULL;
}

void* c(void* p) {
    int i;
    for(i = 0; i < 200; i++) {
        sem_wait(turnC);
        printf("C");
        fflush(stdout);
        sem_post(turnD);
    }
    return NULL;
}

void* d(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        sem_wait(turnD);
        sem_wait(turnD);
        printf("D");
        fflush(stdout);
        sem_post(turnA);
        sem_post(turnA);
        sem_post(turnA);
    }
    return NULL;
}

int main() {
    pthread_t ID[3];
    
    turnA = sem_open("turnA", O_CREAT, S_IRUSR | S_IWUSR, 3);
    turnB = sem_open("turnB", O_CREAT, S_IRUSR | S_IWUSR, 0);
    turnC = sem_open("turnC", O_CREAT, S_IRUSR | S_IWUSR, 0);
    turnD = sem_open("turnD", O_CREAT, S_IRUSR | S_IWUSR, 0);
    
    pthread_create(&ID[0], NULL, b, NULL);
    pthread_create(&ID[1], NULL, c, NULL);
    pthread_create(&ID[2], NULL, d, NULL);

    a(NULL);

    pthread_join(ID[0], 0);
    pthread_join(ID[1], 0);
    pthread_join(ID[2], 0);
    
    sem_close(turnA);
    sem_close(turnB);
    sem_close(turnC);
    sem_close(turnD);
    sem_unlink("turnA");
    sem_unlink("turnB");
    sem_unlink("turnC");
    sem_unlink("turnD");

    return 0;
}
