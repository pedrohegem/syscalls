// Copyright pedrohegem
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static int sigcount;

static void myHandler() {
    sigcount++;
    if (sigcount == 2)  {
        printf("Terminating....\n");
        exit(0);
    }
}

int main() {
    sigcount = 0;
    printf("......\n");
    printf("Producer PID: %ld\n", (long)getpid());
    signal(SIGUSR1, myHandler);

    pause();

    while (1) {
        printf("Producer PID: %ld\n", (long)getpid());
        fflush(stdout);
        sleep(2);
    }

    return 0;
}
