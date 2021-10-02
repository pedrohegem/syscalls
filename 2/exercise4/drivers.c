// Copyright pedrohegem
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    int pid, pid1, pid2;
    if (0 > (pid = fork())) {
        perror("");
        exit(1);
    }

    if (pid == 0) {
        int pid1;
        if ( 0 > execl("./producer", "producer", NULL) ) {
            perror("");
            exit(1);
        }
        exit(0);
    }

    if (0 > (pid1 = fork())) {
        perror("");
        exit(1);
    }
    if (pid1 == 0) {
        if ( 0 > execl("./consumer", "consumer", "file1", NULL) ) {
            perror("");
            exit(1);
        }
        exit(0);
    }

    if (0 > (pid2 = fork())) {
        perror("");
        exit(1);
    }
    if (pid2 == 0) {
        if ( 0 > execl("./consumer", "consumer", "file2", NULL) ) {
            perror("");
            exit(1);
        }
        exit(0);
    }

    sleep(10);
    kill(pid, SIGUSR1);
    kill(pid1, SIGUSR1);
    kill(pid2, SIGUSR1);
    sleep(10);
    kill(pid, SIGUSR1);

    return 0;
}
