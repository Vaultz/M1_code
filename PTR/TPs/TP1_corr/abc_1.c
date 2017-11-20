#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// compile avec
// gcc -Wall abc_1.c -lpthread

void* a(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        printf("A");
        fflush(stdout);
    }
    return NULL;
}

void* b(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        printf("B");
        fflush(stdout);
    }
    return NULL;
}

void* c(void* p) {
    int i;
    for(i = 0; i < 100; i++) {
        printf("C");
        fflush(stdout);
    }
    return NULL;
}

int main() {
    pthread_t ID[2];

    pthread_create(&ID[0], NULL, b, NULL);
    pthread_create(&ID[1], NULL, c, NULL);

    a(NULL);

    pthread_join(ID[0], NULL);
    pthread_join(ID[1], NULL);

    return 0;
}
