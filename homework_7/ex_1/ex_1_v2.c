#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define P 7
#define MAX_CHAR 50000
pthread_mutex_t mutex1, mutex2;
char genome[MAX_CHAR];
int proc_stop, len_genome, proc_num = 0, occ_count = 0;
typedef struct {
  char str[MAX_CHAR];
  int searchlength;
} gen_search;

void loadstr(const char *filename, char *str) {
  FILE *fp;
  if ((fp = fopen(filename, "r")) == NULL) {
    perror("fopen failure");
    exit(EXIT_FAILURE);
  }
  fscanf(fp, "%s", str);
  fclose(fp);
}

void *search(void *arg) {
  int start, stop, my_num,corr=0;
  gen_search *proc_arg = (gen_search *)arg;
  pthread_mutex_lock(&mutex1);
  proc_stop += proc_arg->searchlength;
  proc_num++;
  if (proc_num == P){
    stop = len_genome;
    corr = len_genome - P * proc_arg->searchlength;
  } else
    stop = proc_stop;
  pthread_mutex_unlock(&mutex1);
  start = stop - proc_arg->searchlength-corr;
  printf("[%lu] Searching from pos. %d to %d with step of %lu\n",
         pthread_self(), start, stop, strlen(proc_arg->str));
  for (int i = start; i < stop; i++) {
    if (strncmp(proc_arg->str, &genome[i], strlen(proc_arg->str)) == 0) {
      // printf("[%lu] %s found! (pos: %d)\n", pthread_self(), proc_arg->str,
      // i);
      pthread_mutex_lock(&mutex2);
      occ_count++;
      pthread_mutex_unlock(&mutex2);
    }
  }
  pthread_exit(NULL);
}

int main() {
  gen_search arg;
  pthread_mutex_init(&mutex1, NULL);
  pthread_mutex_init(&mutex2, NULL);
  pthread_t *tid;
  tid = (pthread_t *)malloc(P * sizeof(pthread_t));
  // load genome and sequence in memory
  loadstr("genome.dat", genome);
  loadstr("sequence.dat", arg.str);
  // calculate portion of genome to scan per process
  len_genome = strlen(genome);
  arg.searchlength = (int)floor(len_genome / P);

  for (int i = 0; i < P; i++) {
    pthread_create(tid + i, NULL, search, (void *)&arg);
  }
  for (int i = 0; i < P; i++) {
    pthread_join(*(tid + i), NULL);
  }
  printf("Total occurences of %s: %d\n", arg.str, occ_count);
  pthread_mutex_destroy(&mutex1);
  pthread_mutex_destroy(&mutex2);
  free(tid);
}