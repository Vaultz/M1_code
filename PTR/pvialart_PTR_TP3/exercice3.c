#include <stdio.h>   // printf
#include <pthread.h> // pthread
#include <unistd.h>  // sleep
#include <stdlib.h>  // srand
#include <time.h>    // time

// Reprise de pthread_rwlock_example, avec l'ajout des variables et instructions permettant d'adopter le modèle équilibrer
// On supprime les wrlock et rwlock, et on les remplace par des mutex

//pthread_rwlock_t lock;
char data = 'A';

// Déclaration de trois mutex et d'un int nécessaires à la mise en place d'un système équilibré
int nb_lecteurs = 0;
pthread_mutex_t reading;
pthread_mutex_t writing;
pthread_mutex_t fifo;

void* lecteur(void *arg) {
	int id = *((int*) arg);
	int i;

	for (i = 0; i < 5; i++) {
		printf("Lecteur %d veut consulter la ressource\n", id);

		// Lock mutex : ajout de la notions de chances égales
		pthread_mutex_lock(&fifo);
		pthread_mutex_lock(&reading);
		nb_lecteurs += 1;

		// chaque lecteur verrouille l'accès à l'écriture
		if(nb_lecteurs ==1) {
			pthread_mutex_lock(&writing);		
		}
		// Puis quand l'écriture est verrouillée : on permet aux lecteurs d'accéder à la variable
		pthread_mutex_unlock(&reading);
		pthread_mutex_unlock(&fifo);

		//pthread_rwlock_rdlock(&lock);
		printf("Lecteur %d consulte la ressource : %c\n", id, data);
		sleep(rand() % 2); // on simule un traitement long
		printf("Lecteur %d a fini de consulter la ressource : %c\n", id, data);
		//pthread_rwlock_unlock(&lock);

		// Unlock mutex
		pthread_mutex_lock(&reading);
		nb_lecteurs -= 1;
		if (nb_lecteurs == 0) {
			pthread_mutex_unlock(&writing);
		}
		pthread_mutex_unlock(&reading);


		sleep(rand() % 3); // le lecteur fait une pause
	}
	printf("Lecteur %d : s'en va\n", id);
    return NULL;
}

void* ecrivain(void *arg) {
	int id = *((int*) arg);
	int i;

	for (i = 0; i < 5; i++) {
		printf("Ecrivain %d veut modifier la ressource\n", id);

		// Blocage mutex 
		pthread_mutex_lock(&fifo);
		pthread_mutex_lock(&writing);
		pthread_mutex_unlock(&fifo);

		//pthread_rwlock_wrlock(&lock);
		printf("Ecrivain %d modifie la ressource : %c\n", id, data);
		sleep(rand() % 2); // on simule un traitement long
		data++;
		printf("Ecrivain %d : maintenant la ressource est %c\n", id, data);
		//pthread_rwlock_unlock(&lock);

		pthread_mutex_unlock(&writing);
		sleep(rand() % 3); // l'écrivain fait une pause
	}
	printf("Ecrivain %d : s'en va\n", id);
    return NULL;
}

int main() {
	int i, nb[8];
	srand(time(NULL)); // initialisation de rand
    pthread_t tid[8];

    //pthread_rwlock_init(&lock, NULL);
	for (i = 0; i < 5; i++) {
		nb[i] = i;
    	pthread_create(&tid[i], NULL, lecteur, (void*) &nb[i]);
    }
	for (i = 0; i < 3; i++) {
		nb[i+5] = i;
		pthread_create(&tid[i+5], NULL, ecrivain, (void*) &nb[i+5]);
    }

    for (i = 0; i < 8; i++) {
		pthread_join(tid[i], NULL);
    }
    puts("Consultation et modifications terminées");

	//pthread_rwlock_destroy(&lock);
	pthread_mutex_destroy(&reading);
	pthread_mutex_destroy(&writing);
	pthread_mutex_destroy(&fifo);
    return 0;
}
