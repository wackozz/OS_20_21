/*
v1
06 April 2021
coded by wackoz (s279895@)
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

void title() {
    printf("\e[1m     ______________________________________\n");
    printf("\e[1m   /\t\t\t\t\t   /\n");
    printf("\e[1m  / Homework 03 - Exercize no. 4 (server) /\n");
    printf("\e[1m /_______________________________________/\n\n");
}
void whoAmI() {
    printf(CYN "[Process %d, child of %d.]:", getpid(), getppid());
    printf(RESET);
}

int main() {
    title();
    int shmid;
    char tmp[1024];
    char* msg;
    key_t key;
    key = ftok("/tmp", 'A');  //anyfile

    if ((shmid = shmget(key, 1024, IPC_CREAT | 0666)) < 0) {
        perror(RED "SHMID: ");
        exit(EXIT_FAILURE);
    }
    if ((msg = (char*)shmat(shmid, NULL, 0)) < 0) {
        perror(RED "SHMAT: ");
        exit(EXIT_FAILURE);
    }

    printf("Ready to send data:");
    do {
        scanf("%s", tmp);
        strcpy(msg,tmp);
        printf("%s, command sent.\n", msg);
    } while (strcmp(msg, "exit") != 0);
    shmdt(msg);
    return 0;
}