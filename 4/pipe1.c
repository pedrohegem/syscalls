// Copyright pedrohegem
/* Design a program that creates 2 processes A and B, working B as an echo server
   it reads from a pipe the data A sends, then writes the data on another pipe so
   A can read and print that data.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int     fd[2], fd1[2], nbytes, i;
  pid_t   pid;
  int dato = 1;

  // Creating both pipes
  pipe(fd);
  pipe(fd1);
  switch (pid = fork()) {
      case -1:  // Error
        perror("");
        exit(1);

      case 0:  // Child process
        i = 0;
        close(fd[1]);
        close(fd1[0]);

        while (i < 10) {
            nbytes = read(fd[0], &dato, sizeof(int));  // Reads from pipe 0
            write(fd1[1], &dato, sizeof(int));  // Writes in pipe 1
            i++;
        }
        break;

      default:  // Parent process
        close(fd[0]);
        close(fd1[1]);

        while (dato < 11) {
            write(fd[1], &dato, sizeof(int));  // Writes in pipe 0
            nbytes = read(fd1[0], &dato, sizeof(int));  // Reads from pipe 1
            printf("Received string: %d\n", dato);
            dato++;
        }
        exit(0);
  }
}
