#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NB_FOURNEE 4
#define NB_GATEAUX 4
#define NB_ENFANTS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gateaux_prets = PTHREAD_COND_INITIALIZER;
int gateaux = 0;
int stop = 0;

void* enfant(void *arg) { // mange les gateaux
    int id = *((int*) arg);
    int mange = 0;      // combien de gâteaux mangés ?
    while(1) {
        pthread_mutex_lock(&mutex);
        while(gateaux == 0) {
            if (stop == 1) {    // est-ce que le parent a fini de produire ?
                pthread_mutex_unlock(&mutex);
                goto fin;
            }
            pthread_mutex_unlock(&mutex);
            usleep(100000);
            pthread_mutex_lock(&mutex);
        }
        if (gateaux > 0) {
            gateaux--;
            mange++;
            printf("L'enfant %d a mangé un gateau\n", id);
        } else {
            printf("L'enfant %d n'a pas eu de gateau\n", id);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    fin :
    return *((void**) &mange);
}

void* parent(void *arg) { // prépare les gateaux
    int i;
    for (i = 0; i < NB_FOURNEE; i++) {
        pthread_mutex_lock(&mutex);
        gateaux += NB_GATEAUX;
        printf("Le parent a préparé des gateaux\n");
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    pthread_mutex_lock(&mutex);
    stop = 1;     // indique qu'il n'y a plus de gâteaux
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    int i, nb[NB_ENFANTS];
    void* ret[NB_ENFANTS];
    pthread_t tid[NB_ENFANTS + 1];

    for (i = 0; i < NB_ENFANTS; i++) {
        nb[i] = i;
        pthread_create(&tid[i], NULL, enfant, (void*) &nb[i]);
    }
    pthread_create(&tid[i], NULL, parent, NULL);

    for (i = 0; i < NB_ENFANTS; i++) {
        pthread_join(tid[i], &ret[i]);
        printf("L'enfant %d a mangé %d gateaux\n", i, *((int*) &ret[i]));
    }
    pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}
