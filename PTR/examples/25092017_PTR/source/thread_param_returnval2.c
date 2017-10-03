#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

// Discussion : comprendre paramètres et retour

void* f(void* p) {
  int a = *(int*) p;
  int b = *(int*) (p + sizeof(int));
  printf("thread %d %d %d\n", (int)pthread_self(), a, b);

  int * t = malloc(2 * sizeof(int)); // j'alloue de la mémoire
  t[0] = 31 + a;
  t[1] = 32 + b;
  return (void*) t; // je retourne un pointeur sur la mémoire allouée
}

int main() {
  pthread_t id;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  int v[2];
  v[0] = 1;
  v[1] = 2;
  pthread_create(&id, &attr, f, v);
  int * ret;
  pthread_join(id, (void**) &ret);
  printf("ret %d %d\n", ret[0], ret[1]);
  free(ret); // je libère la mémoire allouée dans le thread
  return 0;
}
