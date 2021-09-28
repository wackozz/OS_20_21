#include <malloc.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

double tot = 0;
int count = 0;
pthread_mutex_t m;
void* series(void* x) {
    double* base = (double*)x;
  pthread_mutex_lock(&m);
  tot += (int)pow(*base, (double)count);
  printf("Current tot= %.2f [it=%d]\n",tot,count);
  count++;
  pthread_mutex_unlock(&m);
  pthread_exit(NULL);
}

int main() {
  int N = 0;
  double x = 0;
  pthread_mutex_init(&m, NULL);
  printf("Multithread calculation of x^i product with i from 0 to N\n");
  printf("Insert N,x:\n");
  scanf("%d,%lf", &N, &x);
  pthread_t *tid;
  tid = (pthread_t*) malloc(N*sizeof(pthread_t));
    for (int i = 0; i < N+1; i++) {
       pthread_create(tid+i,NULL,series,(void*)&x);
    }
    for (int i = 0; i < N+1; i++) {
       pthread_join(*(tid+i),NULL);
    }
    printf("Tot = %.2f\n",tot);
    pthread_mutex_destroy(&m);
    free(tid);
}