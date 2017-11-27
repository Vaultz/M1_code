/* creation d’un pthread detache
 * la fin du main met fin a tous les threads
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* funcPth0(void* _av) {
    pthread_t id = pthread_self();
    int i;
    for (i = 0; i < 10; i++) {
        printf("thread %d : %d\n", (int) id, i);
        sleep(1);
    }
    printf("thread %d : Je termine\n", (int) id);
    // PTHREAD_CREATE_DETACHED :
    // les ressources qui étaient allouées sont automatiquement libérées
    return 0;
}

int main(void) {
    pthread_t pth;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    pthread_create(&pth, &attr, &funcPth0, NULL);

    pthread_attr_destroy(&attr); // attr inutile une fois le thread créé
    
    sleep(3);  // travail quelconque
    
    pthread_t id = pthread_self();
    printf("thread %d : je retourne\n", (int) id);

    //return 0; // le processus retourne et ses threads sont détruits
    pthread_exit(0); // le processus retourne mais ses threads restent en vie
}
