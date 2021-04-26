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

#define SIZE 8196
#define NAME "/my_shm1"

void child_receive() {
  int shm_fd;
  void* shm_area;
  int number = -1;
  int tmpNumber = -1;

  shm_fd = shm_open(NAME, O_RDWR, 0);
  if (shm_fd == -1) {
    perror("Error attachment in send_it: %s");
    exit(errno);
  }
  shm_area = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_area == MAP_FAILED) {
    perror("Faile mmap:%s");
    exit(errno);
  }
  do {
    memcpy(&number, shm_area, sizeof(int));
    if (number != tmpNumber) {
      printf("Child read number: %d\n", number);
      tmpNumber = number;
    }
  } while (number);
  if (munmap(shm_area, SIZE) == -1) {
    perror("Error in unmap in recieve:%s");
    exit(errno);
  }
  exit(0);
}

int main() {
  FILE* fp;
  int number;
  pid_t pid;
  int shm_fd;
  void* shm_area;

  pid = fork();
  if (pid < 0) {  // Error
    perror("Error fork(1):%s");
  } else if (pid == 0) {  // Child 1
    sleep(0.5);
    child_receive();
    // printf ("child 1\n");
  } else {  // parent

    shm_fd = shm_open(NAME, O_RDWR | O_CREAT, 0666);
    if (shm_fd == -1) {
      perror("Error shm_open_1: %s");
      exit(errno);
    }
    ftruncate(shm_fd, SIZE);
    printf("Shared memory Region successfully created\n");

    fp = fopen("number.dat", "r");
    if (fp == NULL) {
      perror("Error opening file: %s");
      exit(errno);
    }
    shm_area = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_area == MAP_FAILED) {
      perror("Faile mmap:%s");
      exit(errno);
    }
    while (fscanf(fp, "%d", &number) == 1) {
      printf("Read number: %d\n", number);
      memcpy(shm_area, &number, sizeof(int));

      sleep(1);
      // printf ("parent:%d", number);
    }
    if (munmap(shm_area, SIZE) == -1) {
      perror("Error in unmap in send:%s");
      exit(errno);
    }
  }

  fclose(fp);
  wait(NULL);
  if (shm_unlink(NAME) == -1) {
    perror("error shm_unlink: %s");
    exit(errno);
  }
  printf("Shared memory area removed successfully\n");
}