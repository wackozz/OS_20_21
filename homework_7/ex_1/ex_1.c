#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "terminalcode.h"

#define MAX_L 50
#define MAX_P 8
#define MAX_G 50000000

char genome[MAX_G], query[MAX_G];

struct thread_parms {
  int N;
  int start;
  int stop;
  int Tnum;
  int string_loc;
};

void* runner(void* param) {
  struct thread_parms* p = (struct thread_parms*)param;
  char* genomep = &genome[p->start];
  char genome_part[MAX_G];
  int found = -1;
  printf(MAG "Thread %d running:\n", p->Tnum);
  p->string_loc = -1;
  for (int i = 0; i < p->stop; i++) {
    memcpy(genome_part, genomep, p->N);
    genome_part[p->N] = '\0';
    // printf(RESET "[T.%d], query = %s, current = %s\n", p->Tnum,
    // query,genome_part);
    if (strcmp(query, genome_part) == 0) {
      found = i;
      i = p->stop;
    }
    genomep++;
  }
  if (found > -1) {
    p->string_loc = p->start + found;
    printf(GRN "[T.%d] MATCH @ %d\n", p->Tnum, p->string_loc);
  }
  pthread_exit(NULL);
}

int main() {
  // variable declaration
  pthread_t tid[MAX_P];
  struct thread_parms thread_p[MAX_P];
  FILE *fpgenome, *fpseq;
  int P = 0, found = 0;
  int t_created = 0, tot_op, step, start = 0;
  char genome_filename[MAX_L], seq_filename[MAX_L];
  strcpy(genome_filename, "genome.dat");
  strcpy(seq_filename, "sequence.dat");
  // genome acquisition
  /*
  printf("Insert the genome filename: ");
  fgets(genome_filename, MAX_L, stdin);
  strtok(genome_filename, "\n");
  */
  if (!(fpgenome = fopen(genome_filename, "r"))) {
    perror(RED "Cannot open the file");
    return 0;
  }
  fgets(genome, MAX_G, fpgenome);
  strtok(genome, "\n");
  fclose(fpgenome);

  // sequence acquisition

  /*
  printf("Insert the sequence filename: ");
  fgets(seq_filename, MAX_L, stdin);
  strtok(genome_filename, "\n");
  */
  if (!(fpseq = fopen(seq_filename, "r"))) {
    perror(RED "Cannot open the file");
    return 0;
  }
  fgets(query, MAX_G, fpseq);
  strtok(query, "\n");
  fclose(fpseq);

  // thread number
  printf("Insert the number of threads: ");
  scanf("%d", &P);
  if (P < 1 || P > MAX_P) {
    printf(RED "Cannot create the required number of threads.\n");
    return 0;
  }

  int query_len = strlen(query);
  printf(CYN "Lenght of the query: %d\n", query_len);
  // total number of operations
  tot_op = strlen(genome) - strlen(query) + 1;
  printf(CYN "Tot. number of operation: %d\n", tot_op);
  // approximate number of step for each thread
  step = (int)ceil((float)tot_op / P);
  printf(CYN "Step per thread: %d\n", step);

  // thread parameters
  for (int i = 0; i < P; i++) {
    t_created++;
    thread_p[i].N = query_len;
    thread_p[i].start = start;
    thread_p[i].Tnum = i;
    if (tot_op < step) {
      thread_p[i].stop = tot_op;
      i = P - 1;
    } else {
      thread_p[i].stop = step;
    }
    start += step;
    tot_op -= step;
  }

  // thread creation
  for (int i = 0; i < t_created; i++) {
    pthread_create(&tid[i], NULL, runner, (void*)&thread_p[i]);
  }

  for (int i = 0; i < t_created; i++) {
    // length of the query
    thread_p[i].N = query_len;
    pthread_join(tid[i], NULL);
    if (thread_p[i].string_loc > -1) {
      printf(GRN "[T.%d] SEQUENCE ''%s'' FOUND @ %d\n", thread_p[i].Tnum, query,
             thread_p[i].string_loc);
      found = 1;
    }
  }
  if (!found) {
    printf(RED "SEQUENCE NOT FOUND.\n");
  }
  return 0;
}