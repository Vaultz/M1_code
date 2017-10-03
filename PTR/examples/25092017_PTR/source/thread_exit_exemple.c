#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

/* fonction de nettoyage */
void clean_mutex(void * data) {
    pthread_mutex_unlock(&mutex);
    printf("Thread %d a libéré le mutex\n", (int) pthread_self());
}

/* fonction de nettoyage */
void clean_buffer(void * data) {
    int * buffer = (int*) data;
    free(buffer);
    printf("Thread %d a libéré la mémoire allouée\n", (int) pthread_self());
}

/* thread allouant de la mémoire, bloquant le mutex et mettant trop de temps à le libérer */
void* th0(void* p) {
    int * buffer = malloc(1024 * sizeof(int));
    printf("Thread %d a alloué de la mémoire et attend le mutex\n", (int) pthread_self());

    pthread_cleanup_push(clean_buffer, buffer); // empile une fonction de nettoyage
    //pthread_cleanup_push(clean_mutex, NULL); // empile une fonction de nettoyage

    pthread_mutex_lock(&mutex);
    printf("Thread %d a bloqué le mutex\n", (int) pthread_self());
    sleep(6); // simule un traitement trop long
    pthread_mutex_unlock(&mutex);
    printf("Thread %d se termine normalement après avoir libéré le mutex\n", (int) pthread_self());

    //pthread_cleanup_pop(0); // retire la fonction clean_mutex sans l'executer
    pthread_cleanup_pop(1); // retire la fonction clean_buffer et l'exécute
    pthread_exit((void*)3); // sortie theorique propre
    // appels implicites pthread_cleanup_pop eventuels
    // 3 sert de marqueur de sortie
    return NULL; // superflu
}

/* thread bloquant le mutex et le libérant rapidement */
void* th1(void* p) {

    pthread_cleanup_push(clean_mutex, NULL); // empile une fonction de nettoyage

    printf("Thread %d attend le mutex\n", (int) pthread_self());
    pthread_mutex_lock(&mutex);
    printf("Thread %d a bloqué le mutex\n", (int) pthread_self());
    pthread_mutex_unlock(&mutex);
    printf("Thread %d se termine normalement après avoir libéré le mutex\n", (int) pthread_self());

    pthread_cleanup_pop(1);
    pthread_exit((void*)3); // sortie theorique propre
    // appels implicites pthread_cleanup_pop eventuels
    // 3 sert de marqueur de sortie
    return NULL; // superflu
}

int main(void) {
    pthread_t pth[3];
    void* ret;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&pth[0], NULL, th0, NULL); // thread qui va passer
    pthread_create(&pth[1], NULL, th0, NULL); // thread qui va être intérrompu
    sleep(1);
    pthread_create(&pth[2], NULL, th1, NULL); // thread qui attend le mutex

    printf("Attend 10 secondes et stoppe les threads\n");
    sleep(10); // simule une attente avant de considérer les threads plantés
    pthread_cancel(pth[0]); // attention le thread est stoppé brutalement
    pthread_cancel(pth[1]); // attention le thread est stoppé brutalement

    printf("Les threads ont été priés de s'arrêter\n");

    pthread_join(pth[0], (void**)&ret);
    printf("retour du thread %d : %d\n", (int) pth[0], *((int*) &ret));
    pthread_join(pth[1], (void**)&ret);
    printf("retour du thread %d : %d\n", (int) pth[1], *((int*) &ret));
    pthread_join(pth[2], (void**)&ret);
    printf("retour du thread %d : %d\n", (int) pth[2], *((int*) &ret));

    pthread_mutex_destroy(&mutex);
    return 0;
}
