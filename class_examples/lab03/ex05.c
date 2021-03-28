#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//Create a shared memory regione between parent and child to share an array of integer with the sizer of 2 a[2];

int main() {
    int *a;
    pid_t pid;
    int i,status;
    int shmid;  //shared mem id

    //create a a new shared memory between child and parent.

    shmid = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);  //666 = permission of read and write

    if (shmid < 0) {
        printf("Error is shmget\n");
        exit(-1);
    }

    a = (int *)shmat(shmid, NULL, 0);

    pid = fork();

    if (pid < 0) {  //error
        printf("Error in child creation \n");
        exit(-1);
    }
    if (!pid) {  //child
        sleep(4);
        printf("Child reads: a[0]: %d and a[1]: %d", a[0], a[1]);
    } else {  //parent
        a[0] = 0;
        a[1] = 1;

        for (i = 0; i < 10; i++) {
            a[0] = a[0] + a[1];
            a[1] = a[0] + a[1];
            printf("Parent writes: a[0]: %d, a[1] : %d\n", a[0], a[1]);
        }
        wait(&status);
    }
    shmdt(a);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}
