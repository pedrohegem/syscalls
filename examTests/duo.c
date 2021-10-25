#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <signal.h>

void handler() {}

int main () {
    pid_t pidA, pidB;
    int fd;
    char c;
    // ------------------------------------------------------------
    signal(SIGUSR1, handler);
    if (0 > (pidA = fork())) {
        perror("Error forking child A...");
        exit(1);
    }

    if (pidA == 0) {  // Process A
        if( 0 > (pidB = fork())) {
            perror("Error forking child B...");
            exit(1);
        }
        if(pidB == 0) {  // Child B
            // B creates 'contenedor' directory
            if( 0 > mkdir("contenedor", 0700)) {
                perror("Error creating 'contenedor' directoy...");
                exit(1);
            }
            // B creates 'dueto' file
            if( 0 > (fd = open("contenedor/dueto", O_CREAT | O_RDWR, 0700))) {
                perror("Error creating file dueto...");
                exit(1);
            }
            c = 'b';
            // B writes char 'b'
            if (0 > write(fd, &c, sizeof(char))) {
                perror("Error writing B in the file...");
                exit(1);
            }

            kill(pidA, SIGUSR1);
            close(fd);
            exit(0);
        }

        
        pause(); // Process A waits for B to write first

        if( 0 > (fd = open("contenedor/dueto", O_RDWR))) {
            perror("Error opening file dueto...");
            exit(1);
        }

        if (0 > lseek(fd, 0, SEEK_END)) { // Moves file pointer to eof
            perror("Error lseek...");
            exit(1);
        }
        
        c = 'a';
        if (0 > write(fd, &c, sizeof(char))) {
            perror("Error writing A in the file...");
            exit(1);
        }
        // A writes NULL char
        c = 0;
        if (0 > write(fd, &c, sizeof(char))) {
            perror("Error writing A in the file...");
            exit(1);
        }
        close(fd);
        exit(0);
    }
    
    // Father Process waits for children to write before reading.
    if( 0 > wait(NULL)) {
        perror("Error wait...");
        exit(1);
    }

    // Father reads from the file
    if( 0 > (fd = open("contenedor/dueto", O_RDWR))) {
        perror("Error opening file dueto...");
        exit(1);
    }
    printf("File content: ");
    for (int i = 0; i < 3; i++) {
        if(0 > read(fd, &c, sizeof(char))) {
            perror("Reading...");
            exit(1);
        }
        printf("%c", c);
    }
    printf("\n");
    return 0;
}