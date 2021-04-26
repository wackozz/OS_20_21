#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* hello(void* par) {
printf("Thread no. %d created.\n", *(int*)par);
printf("Thread no. %d executing...\n", *(int*)par);
sleep(3);
printf("Thread no. %d terminates.\n", *(int*)par);
pthread_exit(EXIT_SUCCESS);
}

int main() {
  pthread_t tid;
  int par1=1,par2=2;
  pthread_create(&tid, NULL, hello, (void*)&par1);
  pthread_create(&tid, NULL, hello, (void*)&par2);
  sleep(5);
  printf("Main terminates.\n");
  return 0;
}
