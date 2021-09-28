#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t m[2];
pthread_cond_t cond[2];
int k[2] = {0};
int tnum = 0;

void* func(void* arg) {
  int i = 0;
  pthread_mutex_lock(&m[0]);
  i = tnum;
  tnum++;
  pthread_mutex_unlock(&m[0]);
  printf("[%d] Thread no. [%lu]\n", i, pthread_self());

  while (k[i] != 0) {
    pthread_mutex_lock(&m[i]);
    pthread_cond_wait(&cond[i], &m[i]);
    printf("\t[%d] Thread no. [%lu]\n", i, pthread_self());
    pthread_mutex_unlock(&m[i]);
  }

  pthread_exit(NULL);
}

int main() {
  int t2kill = 0, killedall = 0;
  pthread_t tid[3];
  for (int i = 0; i < 3; i++) {
    pthread_cond_init(&cond[i], NULL);
  }
  for (int i = 0; i < 3; i++) {
    pthread_mutex_init(&m[i], NULL);
  }

  for (int i = 0; i < 3; i++) {
    pthread_create(&tid[i], NULL, func, NULL);
  }

  sleep(1);
  while (!killedall) {
    printf("Which thread do you want to kill?\n");
    scanf("%d", &t2kill);
    if (t2kill < 0 || t2kill > 2) {
      t2kill = 0;
    }
    k[t2kill] = 1;
    pthread_cond_signal(&cond[0]);
    pthread_cond_signal(&cond[1]);
    pthread_cond_signal(&cond[2]);
    if (k[0] == 1 && k[1] == 1 && k[2] == 1) {
      killedall = 1;
    }
  }

  for (int i = 0; i < 3; i++) {
    pthread_join(tid[0], NULL);
  }
}