#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_THREADS 100

sem_t sem;

void *file_handling(void *arg) {
  char *filename;
  filename = (char *)arg;

  for (int i = 0; i < 100; i++) {
    sem_wait(&sem);
    printf("Process %lu entering its CS\n", pthread_self());
    FILE *fp;
    int data = 0;
    if ((fp = fopen(filename, "r")) == NULL) {
      perror("");
    }
    while ((fscanf(fp, "%d\n", &data)) != EOF)
      ;
    fclose(fp);
    printf("%d\n", data);
    if ((fp = fopen(filename, "a")) == NULL) {
      perror("");
    }
    fprintf(fp, "%d\n", data + 3);
    fclose(fp);
    sem_post(&sem);
  }

  pthread_exit(NULL);
}

int main() {
  sem_init(&sem, 0, 1);
  int N;
  char filename[50];
  printf("Enter filename:");
  scanf("%s", filename);
  strcat(filename, ".dat");
  FILE *fp;
  if ((fp = fopen(filename, "w+")) == NULL) {
    perror("");
  }
  fclose(fp);

  printf("Insert number of threads:\n");
  scanf("%d", &N);
  pthread_t *thread_id;
  thread_id = (pthread_t *)malloc(N * sizeof(pthread_t));
  for (int i = 0; i < N; i++) {
    pthread_create(thread_id + i, NULL, file_handling, (void *)filename);
  }
  for (int i = 0; i < N; i++) {
    pthread_join(*(thread_id + i), NULL);
  }
  return 0;
}