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
    perror("");
    exit(1);
  }

  if (pid == 0) {  // Child 1: -------------------------------------------------
    int i = 0;

    printf("Child 1 PID: %ld\n", (long)getpid());
    close(fd[1]);
    sleep(1);  // To deal with concurrence
    while (i < 5) {
      nbytes = read(fd[0], &dato, sizeof(int));  // Reads from pipe 0
      printf("Received string from child 1: %d\n", dato);
      i++;
      kill(getppid() + 2, SIGUSR1);
      pause();
    }
    exit(0);
  }

  if (0 > (pid = fork())) {
  perror("");
  exit(1);
  }

  if (pid == 0) {  // Child 2: -------------------------------------------------
    int i = 0;
    printf("Child 2 PID: %ld\n", (long)getpid());

    close(fd[1]);

    while (i < 5) {
      pause();
      nbytes = read(fd[0], &dato, sizeof(int));  // Reads from pipe 0
      printf("Received string from child 2: %d\n", dato);
      i++;
      kill(getppid() + 1, SIGUSR1);
    }
    exit(0);
}

  // Parent: ------------------------------------------------------------------
  close(fd[0]);
  while (dato < 11) {
    write(fd[1], &dato, sizeof(int));
    dato++;
  }
  printf("Parent PID: %ld\n", (long)getpid());
  wait(NULL);

  return 0;
}
