#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_barrier_t debut_reponse;
pthread_barrier_t fin_reponse;

void* th(void* p) {
    int i;
    for (i = 0; i < 5; i++) {
        // attend la question...
        pthread_barrier_wait(&debut_reponse);
        printf("thread %d : HOURA !\n", (int) pthread_self());
        sleep(rand() % 3);
        pthread_barrier_wait(&fin_reponse);
    }
    return NULL;
}

int main() {
    int i;
    srand(time(NULL)); // initialisation de rand (attention : non thread safe!)
    
    pthread_barrier_init(&debut_reponse, NULL, 4); // 3 threads + main thread
    pthread_barrier_init(&fin_reponse, NULL, 4); // 3 threads + main thread
    pthread_t tids[3];
    
    pthread_create(&tids[0], NULL, th, NULL);
    pthread_create(&tids[1], NULL, th, NULL);
    pthread_create(&tids[2], NULL, th, NULL);
    
    printf("main thread : ok, je commence !\n");
    
    for (i = 0; i < 5; i++) {
        printf("main thread : hip hip hip ?\n");
        sleep(1);
        pthread_barrier_wait(&debut_reponse);
        // attend la réponse...
        pthread_barrier_wait(&fin_reponse);
    }

    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    pthread_join(tids[2], NULL);

    pthread_barrier_destroy(&debut_reponse);
    pthread_barrier_destroy(&fin_reponse);
    
    return 0;
}
