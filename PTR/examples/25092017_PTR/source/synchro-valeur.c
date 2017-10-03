#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

// Discussion : perte de cycles
// $ gcc -Wall synchro-valeur.c -lpthread

char TURN;

void* f0(void* _p) {
    int i;
    for(i = 0; i < 100; i++) {
        if(TURN == 'A') {
            puts("A");
            TURN = 'B'; }
    }
    puts("f0_fini");
    return NULL;
}

void* f1(void* _p) {
    int i;
    for(i = 0; i < 100; i++) {
        if(TURN == 'B') {
            puts("B");
            TURN = 'A'; }
    }
    puts("f1_fini");
    return NULL;
}

int main() {
    pthread_t pid[2];
    TURN = 'B';
    pthread_create(&pid[0], NULL, f0, NULL); // pid, attributs, fonction, argument
    pthread_create(&pid[1], NULL, f1, NULL);
    pthread_join(pid[0], 0);
    pthread_join(pid[1], 0);
    
    return 0;
}
