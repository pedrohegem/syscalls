// Copyright pedrohegem
/* Program that invokes a fork syscall in a loop till it fails due to the system memory ran out.
   Prints the total number of clons created.
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int   i = 0;
  pid_t pid;

  while (1) {
    if (0 > (pid = fork())) {  // Checks for errors in the system call
        perror("");
        fprintf(stdout, "Number of clons: %d\n", i);
        return(1);
    }
    i++;
    if (pid == 0) {
      sleep(16);
      break;
    }
    fprintf(stdout, "Parent [%ld]-- created child [%ld]\n", (long)getppid(), (long)pid);
  }
  return 0;
}
