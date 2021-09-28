#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

sem_t sem;

void *thread_func(void *arg) {
  sem_wait(&sem);
  printf("I am thread %lu\n", pthread_self());
  sleep(2);
  printf("I am exiting now\n");
  // end of CS
  sem_post(&sem);
  pthread_exit(NULL);
}
int main() {
  sem_init(&sem, 0, 1);
  pthread_t t1, t2;
  pthread_create(&t1, NULL, thread_func, NULL);
  pthread_create(&t2, NULL, thread_func, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  return 0;
}
