#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

typedef struct {
  int n;
  sem_t * mysem;
  char mysem_name[10];
  sem_t * nextsem;
} th_infos;

void *th(void* p) {
  th_infos info = *(th_infos)*)p;
  int i;
  for (i=0  i<100 ; i++) {
    sem_wait(info.system);
    printf("%d", info.n);
    fflush(stdout);
    sem_post(info.nextsem);
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s [nb_threads]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int num = atoi(argv[1]);
  if (num <=1) {
    fprintf(stderr, "usage: %s [nb_threads > 1]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int i, prev;
  pthread_t * tids = malloc(num)
}
