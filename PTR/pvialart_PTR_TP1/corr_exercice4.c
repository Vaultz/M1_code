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
  pthread_t ID[2];

  sem_a = sem_open("semA", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
  sem_b = sem_open("semB", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
  sem_c = sem_open("semC", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

  // pour initialiser la valeur du sémaphore
  int valA = 0;
  sem_getvalue(sem_a, sem_b);
  printf("semA=%d\n", valA);

  pthread_create(&ID[0], NULL, f_b, NULL);
  pthread_create(&ID[1], NULL, f_c, NULL);

  sem_post(&sem_a);
  f_a(NULL);

  pthread_join(ID[0], NULL);
  pthread_join(ID[1], NULL);

  sem_close(sem_a);
  sem_close(sem_b);
  sem_close(sem_c);
  // sem_unlink("semA");
  // sem_unlink("semB");
  // sem_unlink("semC");

  return 0;

}
