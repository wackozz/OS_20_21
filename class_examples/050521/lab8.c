#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define N 8
int array[N];

struct array_prop {
  int start;
  int end;
};

void* split(void* arg) {
}

void fill_array(int array[]) {
  for (int i = 0; i < N; i++) {
    array[i] = random() % 10;
  }
}

void print_array(int array[]) {
  for (int i = 0; i < N - 1; i++) {
    printf("%d, ", array[i]);
  }
  printf("%d", array[N - 1]);
  printf("\n");
}

int main() {
  fill_array(array);
  print_array(array);
  struct array_prop array_len[1];
  array_len[0].start = 0;
  array_len[0].end = N/2 -1;

  array_len[1].start = N/2;
  array_len[1].end = N;

  pthread_t id[1];
  pthread_create(&id[0], NULL, split, (void*)&array_len);
  pthread_create(&id[1], NULL, split, (void*)&array_len);

  return 0;
}
