// Copyright pedrohegem
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
  while (1) {
      printf("%s \n", argv[1]);
      fflush(stdout);
      sleep(1);
  }
  exit(0);
}
