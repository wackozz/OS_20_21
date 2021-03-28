#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int shmid;
    char *str;
    key_t key;

    key = ftok("/tmp", 'A');
    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid < 0) {
        printf("shmget error in client\n");
        exit(-1);
    }
    str = (char *)shmat(shmid, NULL, 0);
    if (str == (char *)-1) {
        printf("Error in shmat in client\n");
        exit(-1);
    }
    sleep(4);
    printf("Read data from memory %s\n", str);

    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}