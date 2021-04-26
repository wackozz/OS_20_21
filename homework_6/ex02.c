#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MSG_LENGTH 20

int run = 1;
char message[MSG_LENGTH];

void* scan(void* par) {
  {
    if (!strcmp(message, "house") || !strcmp(message, "casa")) {
      printf("House detected.\n");
    }
    if (!strcmp(message, "exit")) {
      run = 0;
    }
    pthread_exit(EXIT_SUCCESS);
  }
}

int main() {
  pthread_t tid;

  do {
    printf("Please enter string:\n");
    fgets(message, MSG_LENGTH, stdin);
    strtok(message, "\n");
    pthread_create(&tid, NULL, scan, NULL);
    sleep(1);
  } while (run > 0);
  printf("Main exiting.\n");
  return 0;
}
