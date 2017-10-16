#include <stdio.h>
#include <memory.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
 int pfd[2]; // to store pipe infos
 if (pipe(pfd) == -1) {
     printf("pipe failed\n");
     return 1;
 }

 int pid = fork();

  if (pid < 0) {
     printf("fork failed\n");
     return 2;
  }
  else if (pid == 0) { // child
     close(pfd[1]);    // close the unused write side
     dup2(pfd[0], 0);  // connect the read side with stdin
     close(pfd[0]);    // close the read side

     execlp("sort", "sort", (char *) 0); // execute sort command
     printf("sort failed");  // if execlp returns, it's an error
     return 3;
  }
  else { // parent
     close(pfd[0]);     // close the unused read side
     dup2(pfd[1], 1);   // connect the write side with stdout
     close(pfd[1]);     // close the write side

     execlp("ls", "ls", (char *) 0); // execute ls command
     printf("ls failed"); // if execlp returns, it's an error
     return 4;
  }
  return 0;
}
