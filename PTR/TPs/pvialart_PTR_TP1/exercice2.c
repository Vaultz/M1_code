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

  sem_init(&sem_a, 0, 0);
  sem_init(&sem_b, 0, 0);
  sem_init(&sem_c, 0, 0);
  // THREAD A
  char a = 'A';
  void* ret0;
  pthread_t thr0;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if(pthread_create(&thr0, &attr, f_a, &a) != 0) {
    fprintf(stderr, "Erreur lors de la création du thread\n");
    exit(EXIT_FAILURE);
  }

  // THREAD B
  char b = 'B';
  void* ret1;
  pthread_t thr1;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if(pthread_create(&thr1, &attr, f_b, &b) != 0) {
    fprintf(stderr, "Erreur lors de la création du thread\n");
    exit(EXIT_FAILURE);
  }

  // THREAD C
  char c = 'C';
  void* ret2;
  pthread_t thr2;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if(pthread_create(&thr2, &attr, f_c, &c) != 0) {
    fprintf(stderr, "Erreur lors de la création du thread\n");
    exit(EXIT_FAILURE);
  }

  sem_post(&sem_a);

  pthread_join(thr0, &ret0);
  pthread_join(thr1, &ret1);
  pthread_join(thr2, &ret2);

  pthread_attr_destroy(&attr);
  sem_destroy(&sem_a);
  sem_destroy(&sem_b);
  sem_destroy(&sem_c);
  return 0;
}
