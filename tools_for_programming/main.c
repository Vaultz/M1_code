/**
   \file main.c
   \brief Short testy project, using Git, Doxygen, Makefile and CMake 
   \author Paul VIALART 

   \author Email  : paulvialart@gmail.com
   \date 10/2017
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "library.h"

#define DEBUG if (1) printf

int main(int argc, char*argv[]) {
  int found = 0;
  int tests = 0;
  int compared;
  int randomN;
  int testUser;


  srand(time(NULL));
  randomN = rand()%100+1;

  while(!found) {
    printf("Please enter a number\n");
    scanf("%d", &testUser);
    compared = compare(testUser, randomN);
    if (compared > 0) {
      printf("Too high !\n");
    }
    else if (compared < 0) {
      printf("Too low...\n");
    }
    else {
      printf("OK ! You made %d test\n", tests);
      found = 1;
    }
    tests++;
  }

  return 0;
}
