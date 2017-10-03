#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* f1(void* p) {
  int i = 0;
  char *letter = (char*)p;
  // int b = *(int*)p;
  while (i<100) {
    // would be better to directly print the variable...
    // printf("B ");
    printf("%c ", *letter);
    fflush(stdout);

    i++;
  }
  return 0;
}

int main() {
  char b = 'B';
  void* ret1;
  pthread_t thr1;
  pthread_attr_t attr1;
  pthread_attr_init(&attr1);
  pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
  if(pthread_create(&thr1, &attr1, f1, &b) != 0) {
    fprintf(stderr, "Erreur lors de la création du thread\n");
    exit(EXIT_FAILURE);
  }

  char c = 'C';
  void* ret2;
  pthread_t thr2;
  pthread_attr_t attr2;
  pthread_attr_init(&attr2);
  pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);
  if(pthread_create(&thr2, &attr2, f1, &c) != 0) {
    fprintf(stderr, "Erreur lors de la création du thread\n");
    exit(EXIT_FAILURE);
  }


  int i = 0;
  while(i < 100) {
    printf("A ");
    fflush(stdout);
    i++;
  }

  pthread_join(thr1, &ret1);
  pthread_join(thr2, &ret2);
  return 0;
}
