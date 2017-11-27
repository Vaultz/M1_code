#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

// SEGFAULTS : use joinable semaphores
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
  pthread_exit(0);
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
  pthread_exit(0);
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
  pthread_exit(0);
}

int main() {

  sem_a = *sem_open("sem_A", O_CREAT, S_IRUSR | S_IWUSR);
  sem_b = *sem_open("sem_B", O_CREAT, S_IRUSR | S_IWUSR);
  sem_c = *sem_open("sem_C", O_CREAT, S_IRUSR | S_IWUSR);

  // THREAD A
  char a = 'A';
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
  pthread_t thr1;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if(pthread_create(&thr1, &attr, f_b, &b) != 0) {
    fprintf(stderr, "Erreur lors de la création du thread\n");
    exit(EXIT_FAILURE);
  }

  // THREAD C
  char c = 'C';
  pthread_t thr2;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if(pthread_create(&thr2, &attr, f_c, &c) != 0) {
    fprintf(stderr, "Erreur lors de la création du thread\n");
    exit(EXIT_FAILURE);
  }

  sem_post(&sem_a);

  pthread_attr_destroy(&attr);

  sem_close(&sem_a);
  sem_close(&sem_b);
  sem_close(&sem_c);
  // sem_unlink("sem_A");
  // sem_unlink("sem_B");
  // sem_unlink("sem_C");

  pthread_exit(0);
}
