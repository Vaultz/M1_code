#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
  pid_t child_pid;
  /* Crée un processus fils. */
  child_pid = fork ();

  if (child_pid > 0) {
    /* Nous sommes dans le processus parent. Attente d'une minute. */
    sleep (60);
  }
  else {
    /* Nous sommes dans le processus fils. Sortie immédiate. */
    exit (0);
  }

  return 0;
}
