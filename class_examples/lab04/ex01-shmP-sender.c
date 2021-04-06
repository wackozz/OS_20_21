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
// Program to share ana array of integer with the sisze os 2 >> int data[2] between two processes

int main() {
    int fd, i;
    int *data;

    fd = shm_open(NAME, O_CREAT | O_RDWR | O_EXCL, 0666);
        
    if (fd<0){
        perror("shm_open: ");
        exit(-1);
    }

    ftruncate(fd,SIZE);
    data = (int *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if (data < 0){
        perror("mmap: ");
    }

    for(i = 0; i<2; i++){
        data[i] = i + 10;
        printf("writing in the sahred memory: data[%d]: %d\n", i, data[i]);
    }


    munmap(data, SIZE);
    close(fd);
    return 0;
}
