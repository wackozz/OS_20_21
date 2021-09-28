#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t m;
int N = 0, notprime = 0;

typedef struct data {
  int start;
  int stop;
} data;

void* isPrime(void* arg) {
  data* trim = (data*)arg;
  for (int i = trim->start; i <= trim->stop; i++) {
    if (N % i == 0) {
      if (notprime == 1) {
        i = trim->stop + 1;
      }
      pthread_mutex_lock(&m);
      notprime = 1;
      pthread_mutex_unlock(&m);
      pthread_exit(NULL);
    }
    /* code here */
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  pthread_mutex_init(&m, NULL);
  // argc check
  int P = 0;  // number, no. of threads
  if (argc == 3) {
    N = atoi(argv[1]);
    P = atoi(argv[2]);
  } else {
    printf("Wrong number of arguments. Usage: %s N P\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  data* arg = (data*)malloc(P * sizeof(data));
  pthread_t* tid = (pthread_t*)malloc(((N / 2) + 1) * sizeof(int));

  if(N == 3 || N==1){
      printf("The number is prime.\n");
      exit(EXIT_SUCCESS);
  }
  if(N==2){
      printf("The number is not prime.\n");
      exit(EXIT_SUCCESS);
  }
  printf("Search ranges from 2 to %d.\n", N / 2);

  if (N / 2 <= P) {
    P = N / 2-1;
  }

  for (int i = 0; i < P - 1; i++) {
    printf("T%d: search from ", i);
    (arg + i)->start = 2 + i * ((N / 2 - 2) / P);
    printf("%d ", (arg + i)->start);
    (arg + i)->stop = 2 + (i + 1) * ((N / 2 - 2) / P) - 1;
    printf("to %d.\n", (arg + i)->stop);
  }
  printf("T%d: search from ", P);
  (arg + P)->start = 2 + (P - 1) * ((N / 2 - 2) / P);
  printf("%d ", (arg + P)->start);
  (arg + P)->stop = N / 2;
  printf("to %d.\n", (arg + P)->stop);

  for (int i = 0; i < P; i++) {
    pthread_create(tid + i, NULL, isPrime, arg + i);
  }

  for (int i = 0; i < P; i++) {
    pthread_join(*(tid + i), NULL);
  }

  if (notprime == 1) {
    printf("The number is not prime.\n");
  } else {
    printf("The number is prime.\n");
  }
  pthread_mutex_destroy(&m);
  free(tid);
}