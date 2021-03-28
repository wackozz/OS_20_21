/*
v1
27 March 2021
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
    printf("\e[1m  /\t Homework 03 - Exercize no. 3     /\n");
    printf("\e[1m /_______________________________________/\n\n");
    
    printf(RESET"Multiprocess computation of the sum from 0 to N of x^i\n");
 
}
void whoAmI() {
    printf(CYN "[Process %d, child of %d.]:", getpid(), getppid());
    printf(RESET);
}

int checkInput(int N) {
    if (N > 5 || N < 0) {
        return 1;
    } else {
        return 0;
    }
}

void geometricSeries(int proc_number, pid_t *pid, float x, double *y) {
    *pid = 1;
    for (int i = 0; i < proc_number + 1; i++) {
        if (*pid > 0) {  //parent
            *pid = fork();
            if (!*pid) {  //child
                y[i] = pow(x, i);
                whoAmI();
                printf("%sChild %d created. Partial val = %.2f\n", RESET, i, y[i]);
            }
        }
    }
}

int main() {
    pid_t pid;
    int N, status, id, shm_id;
    float x;
    double y = 0, *sm_y;
    title();
    printf("Please insert N and x (N max = 5):");
    scanf("%d %f", &N, &x);
    if (checkInput(N)) {
        printf(RED "Error: wrong input value.\n");
        exit(EXIT_FAILURE);
    }
    //shared memory creation
    shm_id = shmget(IPC_PRIVATE, N * sizeof(double), IPC_CREAT | 0777);
    if (shm_id < 0) {
        perror(RED "Error in shmget: ");
        exit(EXIT_FAILURE);
    }
    sm_y = shmat(shm_id, NULL, 0);
    if (*sm_y == -1) {
        perror(RED "Error in shmat.\n");
        exit(EXIT_FAILURE);
    }

    geometricSeries(N, &pid, x, sm_y);

    if (pid > 0) {
        while (wait(&status) > 0) {
        };
        for (int i = 0; i < N + 1; i++) {
            y += sm_y[i];
        }
        if (shmdt(sm_y) != 0) {
            perror(RED "Error in shmat.");
            exit(EXIT_FAILURE);
        }
        whoAmI();
        printf("Geometric series = %.2lf\n", y);
    }
}