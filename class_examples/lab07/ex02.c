#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int status = 0;

void* threadfunc(void* p) {
  sleep(1);
  printf("Hello from thread\n");
  status = 100;
  pthread_exit((void*) &status);
}

int main() {
  int *status2;
  pthread_t tid;
  pthread_create(&tid, NULL, threadfunc, NULL);
  pthread_join(tid, (void**) &status2);

  printf("Status: %d\n",*status2);
}