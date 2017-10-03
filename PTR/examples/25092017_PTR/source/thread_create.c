#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// $ gcc -Wall thread_create.c -lpthread
// Discussion : Processus et thread
// Discussion : thread principal et thread secondaire
// ps maux     (dans une autre console)
// Ctrl+C pour arrêter le processus en force

void* f(void* p) {
  while(1) {
    puts("thread secondaire");
    sleep(1);
  }
  return NULL;
}

int main() {
    pthread_t thr;
    if (pthread_create(&thr, NULL, f, 0) != 0) {
      fprintf(stderr, "Erreur dans pthread_create\n");
      exit(EXIT_FAILURE);
    }
    while (1) {
      puts("thread PRINCIPAL = thread main");
      sleep(1);
    }
    return 0;
}
