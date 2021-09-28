#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* threadfunc(void* p) {
  sleep(4);
  printf("Hello from thread\n");
  pthread_exit(NULL);
}




int main() {
  int* status2;
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&tid, &attr, threadfunc, NULL);
  pthread_join(tid, NULL);
}