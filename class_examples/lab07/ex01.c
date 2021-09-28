#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* threadfunc(void* p) {
  sleep(1);
  printf("Hello from thread\n");
  pthread_exit(NULL);
}

int main() {
  pthread_t tid;

  pthread_create(&tid, NULL, threadfunc, NULL);
  printf("Waiting for the target thread\n");
  pthread_join(tid, NULL);
  printf("Thread with ID = %ld is terminated.\n", tid);
  return 0;
}