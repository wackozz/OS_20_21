#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sharedVar;  // Global variable

void *mythread(void *p) {
  printf("[1] sharedVar is %d thread is %ld\n", sharedVar, pthread_self());
  sharedVar = 111;
  printf("[2]: sharedVar is %d, thread is %ld\n", sharedVar, pthread_self());
  sleep(1);
  sharedVar = 222;
  printf("[3]: sharedVar is %d, thread is %ld\n", sharedVar, pthread_self());
  pthread_exit(NULL);
}

int main() {
  pthread_t tid;
  sharedVar = 1234;
  printf("[1] Main: sharedVar is %d\n", sharedVar);
  pthread_create(&tid, NULL, mythread, NULL);
  sleep(1);
  printf("[2] Main: shared value is %d\n", sharedVar);
  sharedVar = 999;
  printf("[3] Main: shared value is %d\n", sharedVar);
  sleep(2);
  printf("Main done\n");
}