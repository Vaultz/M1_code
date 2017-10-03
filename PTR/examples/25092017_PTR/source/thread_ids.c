#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Discussion : thread principal et thread secondaire
// Discussion : comprendre les ID

void* f(void* p) {
    printf("thread %d, my PID is %d\n", (int) pthread_self(), (int) getpid());
    // note : pthread_self() et  getpid() ne sont pas des int
    // utiliser pthread_equal() pour faire une comparaison entre des pthread_t
    
    return NULL;
}

int main() {
    printf("thread PRINCIPAL %d, my PID is %d\n",
           (int) pthread_self(), (int) getpid());
    f(0);
    
    pthread_t id[10];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int i = 0;
    for(i = 0; i < 10; i++) {
        if (pthread_create(&id[i], &attr, f, 0) != 0) {
            fprintf(stderr, "Erreur dans pthread_create\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0; 
}
