/* creation d’un pthread attendu
 * le main attend la fin du pthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* thread recevant une valeur v
 * et retournant v+1 apres 3 secondes */
void* funcPth0(void* arg) {
    int v = *(int*)arg;
    pthread_t id = pthread_self();
    printf("thread %d\n", (int) id);
    v++;

    sleep(3);

    return (*(void**)&v); // convertion de v en void*
                          // (4 octets pour l'int, 4 octets indéterminés...)
}

int main(void) {
    int v = 3; /* valeur envoyee */
    void* ret; /* valeur recue */

    pthread_t pth;
    // autre manière de définir un thread détaché ou joinable :
    // le préciser dans ses attirbuts
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // PTHREAD_CREATE_JOINABLE est la valeur par défaut
    // (pour un thread détaché : PTHREAD_CREATE_DETACHED)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&pth, &attr, &funcPth0, &v);

    printf("J'attend le thread\n");

    pthread_join(pth, &ret); // ret contient l'entier dans les 4 premiers octets
    // je récupère le retour du thread et seulement maintenant
    // les ressources allouées au thread sont libérées
    printf("envoyee:%d recue:%d\n", v, *(int*)&ret);

    return 0;
}
