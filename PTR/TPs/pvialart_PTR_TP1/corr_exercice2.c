#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_a, sem_b, sem_c;

void* f_a(void* p) {
  int i = 0;
  char *letter = (char*)p;
  while (i<100) {
    sem_wait(&sem_a);
    printf("%c ", *letter);
    fflush(stdout);
    sem_post(&sem_b);
    i++;
  }
  return 0;
}

void* f_b(void* p) {
  int i = 0;
  char *letter = (char*)p;
  while (i<100) {
    sem_wait(&sem_b);
    printf("%c ", *letter);
    fflush(stdout);
    sem_post(&sem_c);
    i++;
  }
  return 0;
}

void* f_c(void* p) {
  int i = 0;
  char *letter = (char*)p;
  while (i<100) {
    sem_wait(&sem_c);
    printf("%c ", *letter);
    fflush(stdout);
    sem_post(&sem_a);
    i++;
  }
  return 0;
}


int main() {
  pthread_t ID[2];

  sem_init(&sem_a, 0, 1);
  sem_init(&sem_b, 0, 0);
  sem_init(&sem_c, 0, 0);

  pthread_create(&ID[0], NULL, b, NULL);
  pthread_create(&ID[1], NULL, C, NULL);

  f_a(NULL);

  pthread_join(ID[0], NULL);
  pthread_join(ID[1], NULL);

  sem_destroy(&sem_a);
  sem_destroy(&sem_b);
  sem_destroy(&sem_c);

  return 0;

}
