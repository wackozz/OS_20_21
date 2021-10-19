#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem;

void* routine(void* arg) {
  char* filename = (char*)arg;
  int num=0;
  FILE* fp;
  for (int i = 0; i < 100; i++) {
    sem_wait(&sem);
    //open file in write mode
    if ((fp = fopen(filename, "r")) == NULL) {
      perror("fopen failure:");
      pthread_exit(NULL);
    }
    //scan file until last num
    while (fscanf(fp, "%d", &num) != EOF)
      ;
    num += 3;
    fclose(fp);
    //open file in append mode
    if ((fp = fopen(filename, "a")) == NULL) {
      perror("fopen failure:");
      pthread_exit(NULL);
    }
    fprintf(fp, "%d\n", num);
    fprintf(stdout, "wrote %d on fp (thread %lu)\n", num,pthread_self());
    fclose(fp);
    sem_post(&sem);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  // initializations
  sem_init(&sem, 0, 1);
  // correctness of argument number
  if (argc != 3) {
    printf(
        "Wrong argument number.\nUsage: %s filename N\n\t-N=Number of "
        "threads\n",
        argv[0]);
    exit(EXIT_FAILURE);
  }
  // variable declaration
  int N = atoi(argv[2]);
  pthread_t* tid = (pthread_t*)malloc(N * sizeof(pthread_t));
  //file creation
  if ((fopen(argv[2], "w+")) == NULL) {
      perror("Error in creating file:");
      pthread_exit(NULL);
    }
  // thread creation
  for (int i = 0; i < N; i++) {
    if (pthread_create(tid + i, NULL, routine, (void*)argv[1]) != 0) {
      perror("pthread_create failure:");
    };
  }
  for (int i = 0; i < N; i++) {
    pthread_join(*(tid + i), NULL);
  }
  sem_destroy(&sem);
  free(tid);
}