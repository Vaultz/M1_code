#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char * argv[]){
  int pid = 0;

  if (argc > 1)
  	pid = atoi(argv[1]);
  else {
  	fprintf(stderr, "usage : %s [pid]\n", argv[0]);
  	exit(1);
  }

  printf("J'envoie un signal SIGUSR1 au processus %d\n", pid);
  kill(pid, SIGUSR1);

  return 0;
}
