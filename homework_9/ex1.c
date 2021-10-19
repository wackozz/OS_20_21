#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t sem;

void* troutine(void* arg) {
  printf("[T. %lu]: Waiting for sem...\n", pthread_self());
  sem_wait(&sem);
  printf("[T. %lu]: Hi!\n", pthread_self());
  sleep(2);
  printf("[T. %lu]: Finished.\n", pthread_self());
  sem_post(&sem);
  pthread_exit(NULL);
}

int main() {
  sem_init(&sem, 0, 1);
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, troutine, NULL);
  pthread_create(&tid2, NULL, troutine, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
}