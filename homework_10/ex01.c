#include <malloc.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex;

int count = 0;

void *reciprocal(void *arg) {
  double *tot = (double *)arg;
  pthread_mutex_lock(&mutex);
  *tot += (double)(1 / pow(2, count));
  printf("i=%d; tot = %.2f\n", count, *tot);
  count++;
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

int main() {
  pthread_mutex_init(&mutex, NULL);
  double tot = 0;
  // receive x as input
  printf("x:");
  int x;
  scanf("%d", &x);
  pthread_t *thread_id;
  thread_id = (pthread_t *)malloc(x * sizeof(pthread_t));
  for (int i = 0; i <= x; i++) {
    printf("inside for: %d\n", i);
    pthread_create(thread_id + i, NULL, reciprocal, &tot);
  }
  for (int i = 0; i <= x; i++) {
    pthread_join(*(thread_id + i), NULL);
  }
  printf("Tot = %.2f\n", tot);
  free(thread_id);
  pthread_mutex_destroy(&mutex);
  return 0;
}