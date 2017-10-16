# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int main(int argc, char * argv[]){

  printf("Je vais lancer system(\"echo bonjour\")\n");
  int status = system("echo bonjour");
  printf("La commande a retourné %d\n", status);

  printf("Je vais lancer system(\"ls -z\")\n");
  status = system("ls -z");
  printf("La commande a retourné %d\n", status);

  return 0;
}
