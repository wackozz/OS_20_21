#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "terminalcode.h"

#define MAX_L 50
#define MAX_P 16
#define MAX_G 50000000

unsigned long num;

struct thread_parms {
  unsigned long start;
  unsigned long stop;
  int Tnum;
  long unsigned found;
};

void* runner(void* param) {
  struct thread_parms* p = (struct thread_parms*)param;
  p->found = 0;
  printf(MAG "Thread %d running: from %lu to %lu\n", p->Tnum, p->start, p->stop);
  for (unsigned long i = p->start; i < p->stop; i++) {
    if (num % i == 0) {
      p->found = i;
      printf(GRN "[T.%d] ZERO REMINDER @ %lu\n", p->Tnum, p->found);
      i = p->stop;
    }
  }
  pthread_exit(NULL);
}

int main() {
  // variable declaration
  pthread_t tid[MAX_P];
  struct thread_parms thread_p[MAX_P];
  unsigned P = 0, task_achieved = 0;
  int t_created = 0;

  // insert number
  printf("Insert the number to analyze: ");
  scanf("%lu", &num);
  // thread number
  printf("Insert the number of threads: ");
  scanf("%d", &P);
  if (P < 1 || P > MAX_P) {
    printf(RED "Cannot create the required number of threads.\n");
    return 0;
  }

  // approximate number of step for each thread
  unsigned long step = (unsigned long)ceil((num/2-1)/P);
  printf(CYN "Step per thread: %lu\n", step);
  unsigned long start = 2;
  unsigned long numcp = num;
  // thread parameters
  for (unsigned i = 0; i < P; i++) {
    t_created++;
    thread_p[i].start = start;
    thread_p[i].Tnum = i;
    if (numcp < step) {
      thread_p[i].stop = start+numcp;
      i = P - 1;
    } else {
      thread_p[i].stop = start+step;
    }
    start = thread_p[i].stop+1;
    numcp -= step;
  }

  // thread creation
  for (int i = 0; i < t_created; i++) {
    pthread_create(&tid[i], NULL, runner, (void*)&thread_p[i]);
  }

  for (int i = 0; i < t_created; i++) {
    // length of the query
    pthread_join(tid[i], NULL);
    if (thread_p[i].found > 0) {
      printf(GRN "[T.%d] DIVISOR FOUND: %lu\n", thread_p[i].Tnum,
             thread_p[i].found);
      task_achieved = 1;
    }
  }
  if (task_achieved == 0) {
    printf(YEL "DIVISOR NOT FOUND, PRIME NUMBER.\n");
  }
  return 0;
}