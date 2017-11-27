/* REPONSE A LA QUESTION
 * Observation :
 * A chaque lancement du programme le nombre de A B C augmente, exemple :
 * premier lancement : ABCABCABCABCABCABC...
 * deuxième lancement : AABBCCAABBCCAABBCC...
 * troisième lancement : AAABBBCCCAAABBB...CCACAABBBCCACAABBB...CACACABBBCACACA...
 * on observe aussi un décalage : les A et C s'entremèlent.
 * Explication :
 * A la fin du premier lancement du programme le sémaphore turnA est à 1. 
 * Au deuxième lancement le sémaphore persistant est récupéré. L'appel à 
 * sem_post augmente sa valeur de 1 : le 1er thread peut donc afficher deux A 
 * coup sur coup, sans attendre le C et donne deux "jetons" au thread B, etc...
 * Au troisième lancement on commence à voir un décalage car après l'affichage 
 * du premier C, le thread qui affiche A peut obtenir le semaphore et affiche
 * son A avant que le 3eme thread affiche le reste des C, etc...
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...

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
    
    turnA = sem_open("turnA", O_CREAT, S_IRUSR | S_IWUSR, 0);
    turnB = sem_open("turnB", O_CREAT, S_IRUSR | S_IWUSR, 0);
    turnC = sem_open("turnC", O_CREAT, S_IRUSR | S_IWUSR, 0);

    // pour visualiser la valeur du semaphore
    int valA = 0;
    sem_getvalue(turnA, &valA); // uniquement pour le déboguage
    printf("turnA=%d\n", valA); // avec de la chance,
                                // on voit qu'il augmente à chaque exécution
    
    pthread_create(&ID[0], NULL, b, NULL);
    pthread_create(&ID[1], NULL, c, NULL);

    sem_post(turnA); // ajoute 1 au semaphore turnA
    a(NULL);

    pthread_join(ID[0], 0);
    pthread_join(ID[1], 0);
    
    sem_close(turnA);
    sem_close(turnB);
    sem_close(turnC);
    // sem_unlink("turnA"); // le semaphore est persistant entre deux exécutions
    // sem_unlink("turnB");
    // sem_unlink("turnC");

    return 0;
}
