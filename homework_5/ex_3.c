#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "terminalcodes.h"

#define MAX_PROC 10

int makeLove(pid_t *pid) {
  pid[0] = fork();
  if (!pid[0]) {
    return 1;
  }
  for (int i = 1; i < MAX_PROC; i++) {
    if (pid[i - 1] > 0) {
      pid[i] = fork();
      if (!pid[i]) {
        return i + 1;
      }
    }
  }
  return 0;
}

int selEvaluation(int sel, pid_t *pid) {
  if (sel > MAX_PROC-1 || sel < 0) {
    return -1;
  } else if (pid[sel] == 0) {
      return -2;
  } else {
    return 0;
  }
}

int activeProcesses(pid_t *pid) {
  int count_proc = 0;
  printf(RESET"List of active processes:");
  for (int i = 0; i < MAX_PROC; i++) {
    if (pid[i] > 0) {
      printf("\n\t[%d] %d", i, pid[i]);
      count_proc++;
    }
  }
  if (count_proc > 0) {
    printf("\nWhich process do you want to kill?\n");
  } else {
    printf(BLD "no current active processes.\n");
  }
  return count_proc;
}

int main() {
  int run = 1, sel_pro = -1, act_proc;
  pid_t pid[MAX_PROC];
  printf(GRN "Creating processes..\n");
  int id = makeLove(pid);

  if (id == 0) {
    act_proc = activeProcesses(pid);
    do {
      scanf("%d", &sel_pro);
      if (!selEvaluation(sel_pro, pid)) {
        kill(pid[sel_pro], SIGQUIT);
        perror(CYN"kill");
        pid[sel_pro] = 0;
      } else {
        printf(RED "Process already killed or wrong number. Retry.\n");
      }

      act_proc = activeProcesses(pid);
      if (act_proc == 0) {
        run = 0;
      }
    } while (run);
  } else {
    srand(getpid() + time(NULL));
    while (1) {
      sleep(rand() % 10);
      printf(MAG"Process %d here telling you stuff.\n", getpid());
      printf(RESET);
    }
  }
  printf(GRN"Task done, qutting.\n");
  return 0;
}