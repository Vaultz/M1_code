#include <stdio.h>   // printf
#include <pthread.h> // pthread
#include <unistd.h>  // sleep
#include <stdlib.h>  // srand
#include <time.h>    // time

char salle = 'A';

// Déclaration de trois mutex et d'un int nécessaires à la mise en place d'un système équilibré
int nb_A = 0;
int nb_B = 0;
pthread_mutex_t group_A;
pthread_mutex_t group_B;
pthread_mutex_t fifo;

void* f_group_A(void *arg) {
	int id = *((int*) arg);
	int i;

	for (i = 0; i < 5; i++) {
		printf("L'étudiant %d (A) veut entrer\n", id);

		pthread_mutex_lock(&fifo);
		pthread_mutex_lock(&group_A);
		nb_A += 1;

		if(nb_A == 1) {
			pthread_mutex_lock(&group_B);		
		}

		pthread_mutex_unlock(&group_A);
		pthread_mutex_unlock(&fifo);

		printf("L'étudiant %d (A) entre dans la salle \n", id);
		sleep(rand() % 2); 
		printf("L'étudiant %d (A) sort de la salle\n", id);
	
		pthread_mutex_lock(&group_A);
		nb_A -= 1;
		if (nb_A == 0) {
			pthread_mutex_unlock(&group_B);
		}
		pthread_mutex_unlock(&group_A);


		sleep(rand() % 3); 
	}
	printf("l'étudiant %d (A) s'en va\n", id);
    return NULL;
}

void* f_group_B(void *arg) {
	int id = *((int*) arg);
	int i;

	for (i = 0; i < 5; i++) {
		printf("L'étudiant %d (B) veut entrer\n", id);

		pthread_mutex_lock(&fifo);
		pthread_mutex_lock(&group_B);
		nb_A += 1;

		if(nb_A == 1) {
			pthread_mutex_lock(&group_A);		
		}

		pthread_mutex_unlock(&group_B);
		pthread_mutex_unlock(&fifo);

		printf("L'étudiant %d (B) entre dans la salle \n", id);
		sleep(rand() % 2); 
		printf("L'étudiant %d (B) sort de la salle\n", id);
	
		pthread_mutex_lock(&group_B);
		nb_A -= 1;
		if (nb_A == 0) {
			pthread_mutex_unlock(&group_A);
		}
		pthread_mutex_unlock(&group_B);


		sleep(rand() % 3); 
	}
	printf("l'étudiant %d (B) s'en va\n", id);
    return NULL;
}


int main() {
	int i, nb[8];
	srand(time(NULL));
    pthread_t tid[8];


	for (i = 0; i < 5; i++) {
		nb[i] = i;
	    	pthread_create(&tid[i], NULL, f_group_A, (void*) &nb[i]);
    }
	for (i = 0; i < 3; i++) {
		nb[i+5] = i;
		pthread_create(&tid[i+5], NULL, f_group_B, (void*) &nb[i+5]);
    }

    for (i = 0; i < 8; i++) {
		pthread_join(tid[i], NULL);
    }
    puts("Les étudiants ont fini");

	//pthread_rwlock_destroy(&lock);
	pthread_mutex_destroy(&group_A);
	pthread_mutex_destroy(&group_B);
	pthread_mutex_destroy(&fifo);
    return 0;
}
