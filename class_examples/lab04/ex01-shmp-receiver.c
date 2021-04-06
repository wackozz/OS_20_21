#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE (2 * sizeof(int))
#define NAME "/shmem"  //name always start with /
// Program to share ana array of integer with the sisze os 2 >> int data[2] between two processes

int main() {
    int fd, i;
    int *data;

    fd = shm_open(NAME, O_CREAT | O_RDONLY, 0666);
    if (fd < 0) {
        perror("shm_open():");
        exit(-1);
    }
    data = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);

    for(i = 0; i<2; i++){
        printf("Reading from the shared memory: The data[%d]: %d\n", i , data[i]);
    }
    munmap(data,SIZE);
    shm_unlink(NAME);
    return 0;
}
