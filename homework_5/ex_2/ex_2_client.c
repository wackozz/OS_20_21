#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "terminalcodes.h"

#define MAX_LENGTH 50

void title();

int main() {
  int fd, running = 1;
  char message[MAX_LENGTH];
  title();

  mkfifo("fifo.p", 0666);
  fd = open("fifo.p", O_WRONLY);
  do {
    fgets(message, MAX_LENGTH, stdin);
    message[strlen(message) - 1] = '\0';
    if (strcmp(message, "exit") == 0) {
      running = 0;
    }
    write(fd, message, MAX_LENGTH);
    printf("Sent %s\n", message);
  } while (running);

  return 0;
}

void title() {
  printf("\e[1m     ______________________________________\n");
  printf("\e[1m   /\t\t\t\t\t   /\n");
  printf("\e[1m  /   Homework 05 - Exercize no. CLIENT   /\n");
  printf("\e[1m /_______________________________________/\n\n");
}