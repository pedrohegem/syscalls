// Copyright pedrohegem
/* Program that waits for the arrival of a signal with pause.
   If it receives either SIGUSR1 or SIGINT, prints the autor name and terminates.
*/
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

static void  handler() {
  printf("\rAuthor: %ld\n", (long)getuid());
  fflush(stdout);
}

int  main() {
  signal(SIGINT, handler);
  signal(SIGUSR1, handler);

  pause();
  return (0);
}
