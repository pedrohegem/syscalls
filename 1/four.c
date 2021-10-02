// Copyright 2021 pedrohegem
/* Program that creates four processes A,B,C and D where A is B's parent, B is it to C and C to D. 
   It invokes the syscall execl to mute 'ps' into the new process to show the hierarchy among them.
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    pid_t pid;
    int statval;

    if (0 > (pid = fork())) {  // Process A forks into B
        perror("");
        return(1);
    }
    if (pid == 0) {
        pid = fork();  // Process B forks into C

        if (pid == 0) {
            pid = fork();  // Process C forks into D

            if (pid == 0) {
                execl("/usr/bin/ps", "ps", "jf", NULL);
                sleep(5);
                exit(0);
            }
            wait(NULL);
            exit(0);
        }
        wait(NULL);
        exit(0);
    }
    wait(NULL);
    exit(0);
}
