// Copyright pedrohegem
/* Program that counts the SIGUSR1 and SIGUSR2 signals it receives. When this program receives the SIGTERM 
   signal, it will show the number of signals SIGUSR1 and SIGUSR2 received and will terminate.
*/
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

static int usr1Count;
static int usr2Count;

void myHandler(int signal) {
  printf("Enters the handle...\n");
  if (signal == 10) {
      usr1Count++;
      printf("Number of SIGUSR1 received: %d\n", usr1Count);
      fflush(stdout);
  }

  if (signal == 12) {
    usr2Count++;
    printf("Number of SIGUSR2 received: %d\n", usr2Count);
    fflush(stdout);
  }
}

int  main() {
  usr1Count = 0;
  usr2Count = 0;

  struct sigaction act;

  // sigaction structure init
  sigemptyset(&act.sa_mask);
  act.sa_handler = myHandler;
  act.sa_flags = 0;

  printf("PID: %ld\n", (long)getpid());
  sigaction(SIGUSR1, &act, 0);
  sigaction(SIGUSR2, &act, 0);

  while (1) {
    pause();
  }

  return (0);
}
