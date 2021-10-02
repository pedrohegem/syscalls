// Copyright pedrohegem
/* Program with a parent and a child. The father waits for the child. The child must send
   the parent a SIGUSR1 signal every two seconds. When the parent has received 4 signals, it 
   kills the child and terminates. 
*/
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static int count;

void myHandler(int s) {
    printf("Enters the handle...\n");
    if (s == 10) {
        count++;
        printf("Received SIGUSR1, counter: %d\n", count);
        fflush(stdout);
    }
}


int main() {
    pid_t pid;
    count = 0;
    struct sigaction act;

    if (0 > (pid = fork())) {
        perror("");
        return(1);
    }
    if (pid == 0) {
        while (1) {
            sleep(2);
            kill(getppid(), SIGUSR1);
        }
    }

    sigemptyset(&act.sa_mask);
    act.sa_handler = myHandler;
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, 0);

    printf("Parent waiting for child...\n");
    while (count < 4) {
        wait(NULL);
    }
    printf("Killing child process...%ld\n", (long)getpid());
    kill(pid, SIGTERM);

    return 0;
}
