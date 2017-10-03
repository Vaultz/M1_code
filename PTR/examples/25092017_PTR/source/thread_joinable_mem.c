/* creation d’un pthread attendu par defaut
 * le main attend la fin du pthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// discussion : libération de la mémoire

void* funcPth0(void* arg) {
    return NULL; // retourne de suite, la pile est conservée
}

int main(void) {
    pthread_t pth; // même variable écrasée à chaque fois
    int nb_start = 0;
    
    while (pthread_create(&pth, NULL, &funcPth0, NULL) == 0) {
        pthread_detach(pth); // pour que la pile soit libérée à la fin du thread
        nb_start++;
        if (nb_start % 5000 == 0) printf("%d\n", nb_start);
    }
    fprintf(stderr, "Echec de création après %d threads lancés\n", nb_start);
    
    // fin du processus : return met fin aux threads qui pourraient encore
    // être "vivants"=> libération de la mémoire
    return EXIT_SUCCESS;
}
