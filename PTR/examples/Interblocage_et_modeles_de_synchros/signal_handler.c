#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler (int signal_number) {
  printf("reception du signal %d\n", signal_number);
}

int main(int argc, char * argv[]){
  int pid = getpid();
  printf("Je suis le processus %d\n", pid);
  printf("J'attends le signal SIGUSR1\n");

  struct sigaction sa;
  sa.sa_handler = &handler; // gestionnaire de signal à utiliser
  sigemptyset(&sa.sa_mask); // on ne bloque pas de signaux spécifiques
  sa.sa_flags = 0; 	        // théoriquement ignoré
  sigaction (SIGUSR1, &sa, NULL);  // mise en place du gestionnaire

  // boucle infinie
  while(1) {
  	printf("#"); fflush(stdout);
  	sleep(1);
  }

  return 0;
}
