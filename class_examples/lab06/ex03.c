#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

void *printhello(void *threadid) {
  int *ip;
  ip = (int *)threadid;
  int tid = *ip;
  printf("Hello from the thread number: %d\n", tid);
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[NUM_THREADS];
  int err;

  for (int i = 0; i < NUM_THREADS; i++) {
    printf("Creating thread number: %d.\n", i);
    err = pthread_create(&threads[i], NULL, printhello, &i);
    if (err != 0) {
      perror("pthread_create:");
      exit(-1);
    }
    sleep(1);
  }
  sleep(5);
  return 0;
}