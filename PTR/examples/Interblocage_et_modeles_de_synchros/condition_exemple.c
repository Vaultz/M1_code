#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_cond_t cond;
pthread_mutex_t mutex; // pour protéger cond

void* th(void* p) {
    int i;
    
    sleep(4); // => les signaux non receptionnés sont perdus...
    
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        // ici on verrifie si des données sont disponibles
        // avant de se mettre en attente
        printf("thread %d : wait signal\n", (int) pthread_self());
        pthread_cond_wait(&cond, &mutex);
        printf("thread %d : signal reçu !\n", (int) pthread_self());
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    int i;
    srand(time(NULL)); // initialisation de rand (attention : non thread safe!)
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t tids[3];
    
    pthread_create(&tids[0], NULL, th, NULL);
    pthread_create(&tids[1], NULL, th, NULL);
    pthread_create(&tids[2], NULL, th, NULL);
    
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        printf("main thread : 1 signal sent\n");
        pthread_cond_signal(&cond);   // debloque un seul
        pthread_mutex_unlock(&mutex);
        
        sleep(3); // travaille
        
        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&cond); // debloque tous
        printf("main thread : signal sent for all\n");
        pthread_mutex_unlock(&mutex);
        
        sleep(3); // travaille
    }

    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    pthread_join(tids[2], NULL);
    
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}
