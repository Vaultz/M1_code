#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// système identique au mutex, à la différence qu'il permet de gérer séparément lecture et écriture

pthread_rwlock_t lock;
char data = 'A';

void* lecteur(void *arg) {
    int id = *((int*) arg);
    int i;

    for (i = 0; i < 5; i++) {
        printf("Lecteur %d veut consulter la ressource\n", id);
        pthread_rwlock_rdlock(&lock);
        printf("Lecteur %d consulte la ressource : %c\n", id, data);
        sleep(rand() % 2); // on simule un traitement long
        printf("Lecteur %d a fini de consulter la ressource : %c\n", id, data);
        pthread_rwlock_unlock(&lock);
        sleep(rand() % 3); // le lecteur fait une pause
    }
    printf("Lecteur %d : s'en va\n", id);
    return NULL;
}

void* ecrivain(void *arg) {
    int id = *((int*) arg);
    int i;

    for (i = 0; i < 5; i++) {
        printf("Ecrivain %d veut modifier la ressource\n", id);
        pthread_rwlock_wrlock(&lock);
        printf("Ecrivain %d modifie la ressource : %c\n", id, data);
        sleep(rand() % 2); // on simule un traitement long
        data++;
        printf("Ecrivain %d : maintenant la ressource est %c\n", id, data);
        pthread_rwlock_unlock(&lock);
        sleep(rand() % 3); // l'écrivain fait une pause
    }
    printf("Ecrivain %d : s'en va\n", id);
    return NULL;
}

int main() {
    int i, nb[8];
    srand(time(NULL)); // initialisation de rand
    pthread_t tid[8];

    pthread_rwlock_init(&lock, NULL);
    for (i = 0; i < 5; i++) {
        nb[i] = i;
        pthread_create(&tid[i], NULL, lecteur, (void*) &nb[i]);
    }
    for (i = 0; i < 3; i++) {
        nb[i+5] = i;
        pthread_create(&tid[i+5], NULL, ecrivain, (void*) &nb[i+5]);
    }

    for (i = 0; i < 8; i++) {
        pthread_join(tid[i], NULL);
    }
    puts("Consultation et modifications terminées");

    pthread_rwlock_destroy(&lock);
    return 0;
}
