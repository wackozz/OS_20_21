#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 1, b = 1;  // global variables
pthread_mutex_t m;

void *thread1(void *param) {
  pthread_mutex_lock(&m);
  printf("First thread:\n");
  printf("a = %d, b=%d\n", a, b);
  a++;
  b++;
  printf("First thread done.\n");
  pthread_mutex_unlock(&m);

  pthread_exit(NULL);
}

void *thread2(void *param) {
  pthread_mutex_lock(&m);
  printf("Second thread:\n");
  printf("a = %d, b=%d\n", a, b);
  a = a * 2 ;
  b = b * 2;
  printf("Second thread done.\n");
  pthread_mutex_unlock(&m);
  pthread_exit(NULL);
}

int main() {
  printf("Initial value: a=%d, b=%d\n",a,b);
  pthread_t tid1, tid2;
  pthread_mutex_init(&m, NULL);
  pthread_create(&tid1, NULL, thread1, NULL);
  pthread_create(&tid2, NULL, thread2, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_mutex_destroy(&m);
  printf("Final value: a=%d, b=%d\n",a,b);
}