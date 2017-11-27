# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int main(int argc, char * argv[]){
    int pid, status = 0;

    pid = vfork();

    if (pid < 0) {
        perror("fork"); // echec lors de la creation
        return -1 ;
    }
    if (pid == 0) { // enfant
        printf("Je suis l'enfant, pid=%d\n", getpid());
        printf("Mon parent est %d\n", getppid());
        execl("/bin/echo", "echo", "bonjour", "!", NULL);
        _exit(0);
    }
    else { // parent
        printf("Je suis le parent, pid=%d\n", getpid());
        printf("J'attends mon enfant\n");
        wait(&status);
        printf("Mon enfant a retourné, status=%d\n", status);
    }

    return 0;
}
