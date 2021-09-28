#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem1;

int running = 1, counter = 0;
void *process(void *arg) {
  while (running) {
    sem_wait(&sem1);
    // start of cs
    counter++;
    // end of cs
    sem_post(&sem1);
  }
  printf("Thread exiting\n");
  pthread_exit(NULL);
}

int main() {
  sem_init(&sem1, 0, 1);
  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, process, NULL) != 0) {
    perror("error in pthread_create().\n");
    exit(-1);
  }

  for (int i = 0; i < 10; i++) {
    sleep(1);
    sem_wait(&sem1);
    printf("main: counter = %d\n", counter);
    sem_post(&sem1);
  }
  running = 0;
  pthread_join(thread_id, NULL);
  return 0;
}