#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NB_PAQUET 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int pileA = NB_PAQUET;
int pileB = 0;
int pileC = 0;

void* monk(void *arg) { // passe les paquets de la pile A à la pile B
    while (pileA > 0) {
        pthread_mutex_lock(&mutex);
        printf("Monk : Je passe un paquet de A à B, laisse moi bien le poser\n");
        pileA--;
        sleep(rand() % 5); // prend du temps à poser le paquet
        pileB++;
        printf("Monk : paquet posé !\n");
        pthread_mutex_unlock(&mutex);
        sleep(1); // pause
    }
    return NULL;
}

void* impatient(void *arg) { // passe les paquets de la pile B à la pile C
    int error;
    
    while (pileC < NB_PAQUET) {
        struct timespec absolute_time;
        
        clock_gettime(CLOCK_REALTIME, &absolute_time); // l'heure courante
        absolute_time.tv_sec += 2;                     // + 2 secondes
        
        // on attend jusqu'à l'échéance après on laisse tomber
        error = pthread_mutex_timedlock(&mutex, &absolute_time);
        assert(error == 0 || error == ETIMEDOUT);
        
        if (error == ETIMEDOUT) {
            puts("Impatient : T'es vraiment trop lent. J'vais prendre un café\n");
            sleep(1); // pause café !
            continue;
        }
        if (pileB > 0) {
            pileB--;
            pileC++;
            puts("J'ai passé un paquet de B à C\n");
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // initialisation de rand
    pthread_t tid[2];
    
    printf("Paquets sur les piles A=%d B=%d C=%d\n", pileA, pileB, pileC);
    
    pthread_create(&tid[0], NULL, impatient, NULL);
    pthread_create(&tid[1], NULL, monk, NULL);
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    
    printf("Paquets sur les piles A=%d B=%d C=%d\n", pileA, pileB, pileC);
    
    return 0;
}
