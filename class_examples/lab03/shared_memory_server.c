#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

//shared memory for server (writer).
int main(){

    int shmid;
    key_t key;
    char *str;
    //key = 1234;
    key = ftak("/tmp", 'A');
    
    shmid = shmget(key, 1024, IPC_CREAT | 0666); //shared memory creation
    if (shmid < 0){
        printf("shmget error\n");
        exit(-1);
    }
    str = (char *) shmat (shmid, NULL, 0); //attaching of the shared memory

    if(str == (char *)-1){
        printf("Shmat error\n");
        exit(-1);
    }

    strcpy(str, "This is a message from server");
    printf("Data is written inthe memory by server\n");
    sleep(4);
    shmdt(str);
    return 0;
}