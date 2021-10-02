// Copyright pedrohegem
/* Program that reads from the standard input the path of a program 
   then creates a child process to execute it.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>

int main(void) {
    char *str1;

// -------------------------------------------------------------------------

    printf("Enter a program's PATH:");
    scanf("%s", str1);

    char *bname = basename(str1);

    if (0 > execl(str1, bname, NULL)) {
        fprintf(stderr, "Error in exec %d\n", errno);
        exit(1);
    }

    exit(0);
}
