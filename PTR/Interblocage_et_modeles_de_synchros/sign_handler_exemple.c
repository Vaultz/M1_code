#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// norme ISO/ANSI du langage C de 1989 :
//  "Seul le type sig_atomic_t garantit l'atomicité de l'accès à l'objet"
sig_atomic_t sigusr1_count = 0;

void handler (int signal_number) {
    printf("reception du signal %d\n", signal_number);
    ++sigusr1_count;
}

// quelque chose de long
int fib (int i) {
    if (i == 0)
        return 0;
    else if (i == 1)
        return 1;
    else
        return fib(i-1) + fib(i-2);
}

int main () {
    // struct sigaction
    // {
    //   void     (*sa_handler)(int); /* Adresse du gestionnaire */
    //   sigset_t sa_mask;            /* Masque des signaux bloques pendant
    //                                 * l'exécution du gestionnaire */
    //   int      sa_flags;           /* Ignore pour l'instant */
    // }

    struct sigaction sa;
    sa.sa_handler = &handler; // gestionnaire de signal à utiliser
    sigemptyset(&sa.sa_mask); // on ne bloque pas de signaux spécifiques
    sa.sa_flags = 0;          // théoriquement ignoré
    sigaction (SIGINT, &sa, NULL);  // mise en place du gestionnaire

    /* Faire quelque chose de long ici.  */
    int i = 0;
    for (i = 0; i < 45; i++)
        printf("fib %d = %d\n", i, fib(i));

    printf ("SIGINT a été reçu %d fois\n", sigusr1_count);
    return 0;
}
