#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#define NAME "/myshmem"
#define SIZE 8196

void title() {
  printf("\e[1m     ______________________________________\n");
  printf("\e[1m   /\t\t\t\t\t   /\n");
  printf("\e[1m  /\t Homework 04 - Exercize no. 1     /\n");
  printf("\e[1m /_______________________________________/\n\n");
}
void whoAmI() {
  printf(CYN "[Process %d, child of %d.]:", getpid(), getppid());
  printf(RESET);
}

void childReceiver() {
  usleep(5000);
  int shm_fd, number = 0, tmpNumber = -1;
  void *shm_area;
  shm_fd = shm_open(NAME, O_RDWR, 0);
  if (shm_fd == -1) {
    whoAmI();
    perror("shm_open: ");
    exit(errno);
  }
  shm_area = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_area == MAP_FAILED) {
    whoAmI();
    perror("mmap: ");
    exit(errno);
  }
  do {
    memcpy(&number, shm_area, sizeof(int));
    if (number !=
        tmpNumber) {  // the problem asks to not rewrite the duplicates
      whoAmI();
      printf("read number: %d\n", number);
      tmpNumber = number;
    }
  } while (number);
  if (munmap(shm_area, SIZE) == -1) {
    whoAmI();
    perror("Error in unmap in receive:%s");
    exit(errno);
  }
  if (shm_unlink(NAME) == -1) {
    whoAmI();
    perror("error shm_unlink: %s");
    exit(errno);
  }
  exit(0);
}

void readFile(void *shm_area) {
  FILE *fp;
  int num;
  fp = fopen("number.dat", "r");
  if (fp == NULL) {  // file check
    whoAmI();
    perror("fopen:");
    exit(EXIT_FAILURE);
  }
  while (fscanf(fp, "%d", &num) != EOF) {
    whoAmI();
    printf("Read number %d\n", num);
    memcpy(shm_area, &num, sizeof(int));
    usleep(500000);
  };
  fclose(fp);
}

int main() {
  title();
  int shm_fd;
  void *shm_area;

  pid_t pid = fork();
  if (pid < 0) {
    perror("Fork: ");
  } else if (!pid) {  // child
    childReceiver();
  } else {  // parent
    shm_fd = shm_open(NAME, O_RDWR | O_CREAT, 0666);
    if (shm_fd == -1) {
      whoAmI();
      perror("shm_open: ");
      exit(errno);
    }
    ftruncate(shm_fd, SIZE);
    shm_area = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_area == MAP_FAILED) {
      whoAmI();
      perror("mmap: ");
      exit(errno);
    }
    readFile(shm_area);
    wait(NULL);
    exit(0);
  }
}