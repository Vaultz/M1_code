# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int main(int argc, char * argv[]){
    int pid, status = 0;

    int var = 33; // cette variable sera copiée

    pid = fork();

    if (pid < 0) {
        perror("fork"); // echec lors de la creation
        return -1 ;
    }
    if (pid == 0) { // enfant
        printf("Je suis l'enfant, pid=%d\n", getpid());
        printf("Mon parent est %d\n", getppid());
        var += 4;
        printf("var chez l'enfant = %d\n", var);
        sleep(3);
        _exit(0); // "it is wrong to call exit
                  // since buffered data would then be flushed twice"
    }
    else { // parent
        printf("Je suis le parent, pid=%d\n", getpid());
        printf("J'attends mon enfant\n");
        var += 1;
        printf("var chez le parent = %d\n", var);
        wait(&status); // la fin du parent ne doit pas faire
                       // de l'enfant un zombie
        printf("Mon enfant a retourné, status=%d\n", status);
    }

    return 0;
}
