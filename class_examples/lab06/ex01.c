#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *myThreadfunc(void *i) {
  int *ip;
  // tranforming a pointer to void into a pointer to integer
  ip = (int *)i;
  int a;
  // saving in to the variable "a", the value contained in the memory region
  // pointed by ip.
  a = *ip;
  printf("This is a function myThreadfunc and the received parameter is %d\n",
         a);
  pthread_exit(NULL);
}

int main() {
  pthread_t tid1;   // thread id
  int number = 10;  // argument for thread function
  pthread_create(&tid1, NULL, myThreadfunc, &number);  // thread creation
  sleep(1);
  return 0;
}