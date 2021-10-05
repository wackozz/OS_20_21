#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t m[4];
pthread_cond_t cond[3];
int kill[3] = {0, 0, 0};
int thread_number = -1;



void* fun(void* arg) {
  int my_num = 0;
  pthread_mutex_lock(&m[0]);
  thread_number++;
  my_num = thread_number;
  pthread_mutex_unlock(&m[0]);
  printf("[%d] Thread %lu\n", my_num, pthread_self());

  while (!kill[my_num]) {
    pthread_mutex_lock(&m[my_num]);
    pthread_cond_wait(&cond[my_num], &m[my_num]);
    printf("\n[%d] Thread %lu.", my_num, pthread_self());
    pthread_mutex_unlock(&m[my_num]);
  }
  printf("\t\tKILLED.");
  pthread_exit(NULL);
}

int main() {
  int to_kill;
  pthread_t tid[3];

  // mutex initializer
  for (int i = 0; i < 4; i++) {
    pthread_mutex_init(&m[i], NULL);
  }
  // condition initializer
  for (int i = 0; i < 3; i++) {
    pthread_cond_init(&cond[i], NULL);
  }
  // thread creation
  for (int i = 0; i < 3; i++) {
    pthread_create(&tid[i], NULL, fun, NULL);
  }
  while (!kill[0] || !kill[1] || !kill[2]) {
    sleep(1);
    printf("\nWhich thread do you want to kill?");
    scanf("%d", &to_kill);
    kill[to_kill] = 1;
    pthread_cond_signal(&cond[0]);
    pthread_cond_signal(&cond[1]);
    pthread_cond_signal(&cond[2]);
  }
  printf("Done..\n");
  // thread join
  for (int i = 0; i < 3; i++) {
    pthread_join(tid[i], NULL);
    printf("join %d\n", i);
  }
}