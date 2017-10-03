#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* thread affichant son tid toute les secondes */
void* funcPth0(void* p) {
    while(1) {
        printf("thread %d\n", (int) pthread_self());
        sleep(1);
    }
    return NULL;
}

int main(void) {
    pthread_t pth;
    pthread_create(&pth, NULL, funcPth0, NULL);
    
    printf("Attend 3 secondes et stoppe le thread\n");
    sleep(3);
    pthread_cancel(pth); // attention le thread est stoppé brutalement
    
    sleep(5); // on attend un peu pour vérifier que le thread n'affiche plus rien
    puts("Exit");
    return 0;
}
