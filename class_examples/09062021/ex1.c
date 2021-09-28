#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex_fuel;
pthread_cond_t cond_fuel;
int fuel = 0;

void* fuelFilling(void* arg) {
  for (int i = 0; i < 50; i++) {
    pthread_mutex_lock(&mutex_fuel);
    fuel += 15;
    printf("Filled fuel..%d\n", fuel);
    pthread_mutex_unlock(&mutex_fuel);
    pthread_cond_signal(&cond_fuel);
  }
  pthread_exit(NULL);
}

void* car(void* arg) {
  pthread_mutex_lock(&mutex_fuel);
  while (fuel < 40) {
    printf("Not enough fuel.. (fuel=%d)\n", fuel);
    pthread_cond_wait(&cond_fuel, &mutex_fuel);
  }
  printf("Getting fuel..\n");
  fuel -= 40;
  printf("Got fuel. Now left:%d\n", fuel);
  pthread_mutex_unlock(&mutex_fuel);
  pthread_exit(NULL);
}

int main() {
  pthread_mutex_init(&mutex_fuel, NULL);
  pthread_cond_init(&cond_fuel, NULL);
  pthread_t th[2];

  for (int i = 0; i < 10; i++) {
    if (i == 1) {
      if ((pthread_create(&th[1], NULL, fuelFilling, NULL)) != 0) {
        perror("Failed to create thread 1:");
      }
    } else {
      if ((pthread_create(&th[2], NULL, car, NULL)) != 0) {
        perror("Failed to create thread 2:");
      }
    }
  }
  pthread_join(th[1], NULL);
  pthread_join(th[2], NULL);
  pthread_mutex_destroy(&mutex_fuel);
  pthread_cond_destroy(&cond_fuel);
}
