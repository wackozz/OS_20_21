#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_t tid[2];
int counter = 0;
pthread_mutex_t lock;

void* function(void* param) {
  pthread_mutex_lock(&lock);
  //beginning of the CS
  printf("The thread starts: counter = %d\n", counter);
  counter++;
  printf("Thread exiting\n");
  //end of the CS
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

int main() {
  int i = 0;
  int error;

  int error_mutex = pthread_mutex_init(&lock, NULL);

  if (error_mutex != 0) {
    perror("Error in mutex creation:");
  }

  while (i < 2) {
    error = pthread_create(&tid[i], NULL, &function, NULL);
    if (error != 0) {
      printf("Cannot create thread\n");
    }
    i++;
  }

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  pthread_mutex_destroy(&lock);
}