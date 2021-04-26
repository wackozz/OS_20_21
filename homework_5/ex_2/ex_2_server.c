#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_LENGTH 50

#include "terminalcodes.h"

void title();

int main() {
  int fd, running = 1;
  char message[MAX_LENGTH];
  title();
  mkfifo("fifo.p", 0666);
  fd = open("fifo.p", O_RDONLY);
  do {
    read(fd, message, MAX_LENGTH);
    printf("Received %s\n", message);
    if (strcmp(message, "exit") == 0) {
      running = 0;
    }
    if (strcmp(message, "casa") == 0 || strcmp(message, "house") == 0) {
      printf("House detected.\n");
    } else {
      system(message);
    }

  } while (running);
  unlink("fifo.p");
  return 0;
}

void title() {
  printf(BLD "     ______________________________________\n");
  printf("/\t\t\t\t\t   /\n");
  printf("/   Homework 05 - Exercize no. SERVER   /\n");
  printf("/_______________________________________/\n\n" RESET);
}