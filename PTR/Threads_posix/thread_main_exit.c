#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* f(void* p) {
  while(1) {
    puts("thread secondaire");
    sleep(1);
  }
  return NULL;
}

int main() {
    pthread_t thr;
    int i;

    if (pthread_create(&thr, NULL, f, 0) != 0) {
      fprintf(stderr, "Erreur dans pthread_create\n");
      exit(EXIT_FAILURE);
    }
    for (i = 0; i < 5; i++) {
      puts("thread PRINCIPAL = thread main");
      sleep(1);
    }

    pthread_exit(0);
}
