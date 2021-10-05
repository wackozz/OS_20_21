#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static const char* GRN = "\033[1;32m";
static const char* RED = "\033[0;31m";
static const char* NC = "\033[0m";

sem_t sem;
pthread_mutex_t mut;
pthread_cond_t cond;

int basket = 0;
int t_num = 0;
int finished = 0;
int lingo = 0;

void* Transform(void* arg) {
  int* M = (int*)arg;
  while (!finished) {
    pthread_mutex_lock(&mut);
    pthread_cond_wait(&cond, &mut);
    printf("%sCalled [T].\n%s",RED,NC);
    if (basket > (*M)) {
      basket -= *M;
      lingo++;
      printf("%s[T]\t[_] Basket: %d (-%d), lingos: %d%s\n", RED, basket, *M,
             lingo, NC);
    }
    pthread_mutex_unlock(&mut);
  }
  pthread_exit(NULL);
}
void* Mine(void* arg) {
    int* M = (int*)arg;
  pthread_mutex_lock(&mut);
  int my_num = t_num;
  t_num++;
  pthread_mutex_unlock(&mut);
  int count = 0;
  int gold = 0;
  while (count < 10) {
    sem_wait(&sem);
    gold = 0;
    gold = rand() % 10;
    sem_post(&sem);
    pthread_mutex_lock(&mut);
    basket += gold;
    printf("%s[T.%d]\t[%d] Basket: %d (+%d), lingos: %d%s", GRN, my_num,
           count + 1, basket, gold, lingo, NC);
    if (gold != 0) {
      count++;
      printf("\n");
    } else {
      count = 10;
      printf("...%s->[EXITING]%s\n", NC, NC);
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);
    
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  // randomseed
  srand(time(NULL));
  // argc, argv check
  if (argc != 3) {
    printf("%s Wrong argument number. Usage %s N M\n", RED, argv[0]);
    exit(EXIT_FAILURE);
  }
  // variable declaration
  int N = atoi(argv[1]);
  int M = atoi(argv[2]);
  pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * (N + 1));
  // initializations
  pthread_mutex_init(&mut, NULL);
  pthread_cond_init(&cond, NULL);
  sem_init(&sem, 0, 2);
  printf("Mine threads: \t%d\nOne lingo = \t%d golds.\n", N, M);
  // thread creation
  // transform process
  pthread_create(tid, NULL, Transform, (void*)&M);
  // mine process
  for (int i = 1; i < N + 1; i++) {
    pthread_create(tid + i, NULL, Mine, (void*)&M);
  }
  // join Mine thread
  for (int i = 1; i < N + 1; i++) {
    pthread_join(*(tid + i), NULL);
  }
  finished = 1;
  pthread_cond_signal(&cond);
  // join Transform thread
  pthread_join(*(tid), NULL);
  printf("\n\n%s---------------------\n", GRN);
  printf("Basket: %d, lingos: %d\n", basket, lingo);
  printf("---------------------\n");
  free(tid);
  sem_destroy(&sem);
  pthread_mutex_destroy(&mut);
}
