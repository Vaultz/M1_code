/* REPONSE A LA QUESTION :
 * Observation :
 * Je vois que les sémapores sem.turnA, sem.turnB et sem.turnC ne 
 * disparaissent pas du répertoire /dev/shm après l'arrêt de mon 
 * programme
 * Explication :
 * Si on commente sem_unlink(), les sémaphores ne sont pas détruits
 * et persistent sous forme de fichiers placés dans le répertoire
 * /dev/hsm sous linux.
 * A la deuxième exécution du programme :
 * Avec le flag O_CREAT, le sémaphore est récupéré car il existe déjà.
 * Avec les flags O_CREAT | O_EXCL, la création du sémaphore échoue car
 * il existe déjà et la valeur retournée est SEM_FAILED. Il faut donc
 * tester cette valeur de retour pour éviter de l'utiliser dans
 * sem_wait ou sem_post.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...

// compile avec
// gcc -Wall abc_3.c -lpthread

sem_t * turnA;
sem_t * turnB;
sem_t * turnC;

void* a(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
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
        printf("B");
        fflush(stdout);
        sem_post(turnC);
    }
    return NULL;
}

void* c(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        sem_wait(turnC);
        printf("C");
        fflush(stdout);
        sem_post(turnA);
    }
    return NULL;
}

int main() {
    pthread_t ID[2];

    // les deux derniers arguments sont optionnels mais
    // plantage possible si on n'indique pas les droits...
    // échec de création des sémaphores ou bien sémaphores inopérants...
//    turnA = sem_open("turnA", O_CREAT | O_EXCL);
//    turnB = sem_open("turnB", O_CREAT | O_EXCL);
//    turnC = sem_open("turnC", O_CREAT | O_EXCL);

    // on obtient une erreur si le sémaphore existe déjà
    turnA = sem_open("turnA", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    turnB = sem_open("turnB", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    turnC = sem_open("turnC", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

    if (turnA == SEM_FAILED || turnB == SEM_FAILED || turnC == SEM_FAILED) {
        puts("Error : sem_open fails");
        exit(EXIT_FAILURE);
    }

    pthread_create(&ID[0], NULL, b, NULL);
    pthread_create(&ID[1], NULL, c, NULL);

    sem_post(turnA); // si le sémaphore est initialisé à zéro
    a(NULL);

    pthread_join(ID[0], NULL);
    pthread_join(ID[1], NULL);

    sem_close(turnA);
    sem_close(turnB);
    sem_close(turnC);
    sem_unlink("turnA");  // à commenter ou décommenter...
    sem_unlink("turnB");
    sem_unlink("turnC");

    return 0;
}
