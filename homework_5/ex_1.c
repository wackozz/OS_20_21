#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "terminalcodes.h"

#define e 2.71828
#define N_MAX 10

int checkN(int N);
int makeLove(int N);
float calculateTerm(int i);
void title();

int main() {
  title();
  int n, id, fd[2];
  float term = 0, term_sum = 0;
  printf(RESET"Series evaluation exercise.\n");

  do {
    printf("Please enter a value between 1 and 10:");
    scanf("%d", &n);
  } while (checkN(n) != 0);
  n--;
  pipe(fd);
  id = makeLove(n);
  if (id != 0) {
    printf(CYN "[Child %d, son of %d]: term %d", getpid(), getppid(), id);
    term = calculateTerm(id);
    printf(" calculated: %f..", term);
    write(fd[1], &term, sizeof(term));
    printf("wrote to PIPE, closing pipe and exiting.\n" RESET);
    close(fd[1]);
    exit(1);
  } else if (id == 0) {
    printf(BLU "[Parent %d]: waiting to receive data from children.\n",
           getpid());
    do {
      read(fd[0], &term, sizeof(term));
      term_sum += term;
      printf(BLU "[Parent %d]: data recived.\n", getpid());
      n--;
    } while (n > 0);
    printf(BLD GRN "The sum is equal to %f. Closing pipe and exiting.\n",
           term_sum);
    close(fd[0]);
  }
  return 0;
}

void title() {
  printf("\e[1m     ______________________________________\n");
  printf("\e[1m   /\t\t\t\t\t   /\n");
  printf("\e[1m  /\t Homework 05 - Exercize no. 1     /\n");
  printf("\e[1m /_______________________________________/\n\n");
}

int checkN(int N) {
  if (N > N_MAX) {
    fprintf(stderr, "Inserted number exceds 10.\n");
    return 1;
  } else if (N < 1) {
    fprintf(stderr, "Inserted number is less than 1.\n");
    return 1;
  } else {
    return 0;
  }
}

int makeLove(int N) {
  pid_t pid = 1;
  for (int i = 0; i < N; i++) {
    if (pid > 0) {  // parent
      printf(BLU "Parent %d, fork executed.\n", getpid());
      pid = fork();
    }
    if (pid == 0) {  // child
      printf(CYN "Child %d, son of %d, with ID=%d.\n", getpid(), getppid(),
             i + 1);
      return i + 1;
    }
  }
  return 0;
}

float calculateTerm(int i) { return pow(e, i) / i; }
