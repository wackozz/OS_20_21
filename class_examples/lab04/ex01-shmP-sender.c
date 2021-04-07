#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE (2 * sizeof(int))
#define NAME "/shmem"
// Program to share ana array of integer with the sisze os 2 >> int data[2]
// between two processes

int main() {
  int fd, i;
  int *data;

  if ((fd = shm_open(NAME, O_CREAT | O_RDWR | O_EXCL, 0666) < 0)) {
    perror("shm_open: ");
    exit(-1);
  }
  
  ftruncate(fd, SIZE); //length of the virtual file
  
  data = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (data < 0) {
    perror("mmap: ");
  }

  for (i = 0; i < 2; i++) {
    data[i] = i + 10;
    printf("Writing in the shared memory: data[%d]: %d\n", i, data[i]);
  }

  munmap(data, SIZE); //unmap
  close(fd); //we don't unlink, a receiver have to read the shared mem
  return 0;
}
