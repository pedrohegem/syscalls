// Copyright pedrohegem
/* Program with a parent process A and to child processes B and C. A process writes 10 numbers into the pipe
   and child processes alternates to read from the pipe.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void  myHandler(int signal) {
}

int main() {
  int     fd[2], nbytes;
  pid_t   pid;
  int dato = 1;
  struct sigaction act;
  sigemptyset(&act.sa_mask);
  act.sa_handler = myHandler;
  act.sa_flags = 0;
  sigaction(SIGUSR1, &act, 0);

  pipe(fd);

  if (0 > (pid = fork())) {
    perror("ERROR");
    exit(1);
  }

  if (pid == 0) {  // Child 1: -------------------------------------------------
    int i = 0;
    pid_t pidChild2;
    printf("Child 1 PID: %d\n", getpid());
    close(fd[1]);
    pause();

    if(0 > read(fd[0], &pidChild2, sizeof(int))) {
      perror("ERROR");
      exit(1);
    }
    
    while (i < 5) {
      if (0 > (nbytes = read(fd[0], &dato, sizeof(int)))) {
        perror("ERROR");
        exit(1);
      }  // Reads from pipe 0
      printf("Received string from child 1: %d\n", dato);
      fflush(stdout);
      if (0 > (kill(pidChild2, SIGUSR1))) {
        perror("ERROR");
        exit(1);
      }
      pause();
      i++;
    }
    exit(0);
  }

  write(fd[1], &pid, sizeof(int));  // Saves the child PID
  if (0 > (pid = fork())) {
  perror("ERROR");
  exit(1);
  }

  if (pid == 0) {  // Child 2: -------------------------------------------------
    int i = 0;
    pid_t pidChild1;
    printf("Child 2 PID: %d\n",getpid());
    close(fd[1]);
    if (0 > read(fd[0], &pidChild1, sizeof(int))) {
      perror("ERROR");
      exit(1);
    }
    if (0 > (kill(pidChild1, SIGUSR1))) {
      perror("ERROR");
      exit(1);
    }

    while (i < 5) {
      pause();
      if(0 > (nbytes = read(fd[0], &dato, sizeof(int)))) {
        perror("ERROR");
        exit(1);
      }   // Reads from pipe 0
      printf("Received string from child 2: %d\n", dato);
      fflush(stdout);
      if (0 > (kill(pidChild1, SIGUSR1))) {
        perror("ERROR");
        exit(1);
    }
      i++;
    }
    exit(0);
}
  if (0 > write(fd[1], &pid, sizeof(int))) {
    perror("ERROR");
    exit(1);
  }  // Saves the child PID

  // Parent: ------------------------------------------------------------------
  close(fd[0]);
  int j = 0;
  while (j < 10) {
    if (0 > write(fd[1], &dato, sizeof(int))) {
      perror("ERROR");
      exit(1);
    }
    dato++;
    j++;
  }
  printf("Parent PID: %d\n", getpid());
  fflush(stdout);
  if(0 > wait(NULL)) {
    perror("ERROR");
    exit(1);
  }

  return 0;
}
