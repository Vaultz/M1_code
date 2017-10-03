#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Discussion : comprendre paramètres et retour

void* f(void* p) {
    int a = *(int*) p;
    int b = *(int*) (p + sizeof(int));
    printf("thread %d %d %d\n", (int)pthread_self(), a, b);
    a += b;
    return *((void**) &a);
}

int main() {
    pthread_t id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int v[2];
    v[0] = 1;
    v[1] = 2;
    pthread_create(&id, &attr, f, v);
    int ret;
    pthread_join(id, (void**) &ret); // ATTENTION !!!
                                     // écriture d'un pointeur dans l'espace d'un int...
    printf("ret %d\n", ret); 
    
    //printf("sizeof(int)=%lu  sizeof(void*)=%lu\n", sizeof(int), sizeof(void*));
    return 0;
}
