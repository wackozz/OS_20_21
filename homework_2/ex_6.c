/*
v1.0
18 March 2021
coded by wackoz (s279895@)
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char *myGetTime() {
    time_t rawseconds;
    time(&rawseconds);
    char *time_str = asctime(localtime(&rawseconds));
    time_str[strlen(time_str) - 1] = 0;  //null for the \n
    return time_str;
}

void title(){
    printf("\e[1m     ______________________________________\n");
    printf("\e[1m   /\t\t\t\t\t   /\n");
    printf("\e[1m  /\t Homework 02 - Exercize no. 6     /\n");
    printf("\e[1m /_______________________________________/\n\n");
}

int main(int argc, char *argv[]) {
    title();
    pid_t pid[5];
    int N = 0, mpy = 0, i = 0, y = 0, status = 0;
    N = atoi(argv[1]);
    if (argc != 2) {
        printf(RED "Error: wrong argument number. Usage: %s N.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (N < 1 || N > 5) {
        printf(RED "Error: N must be comprised between 1 and 5.\n");
        exit(EXIT_FAILURE);
    }

    pid[0] = 1;  //enter the loop
    for (i = 0; i < N; i++) {
        if (pid[0] > 0) {  //parent allowed
            pid[i] = fork();
            if (!pid[i]) {  //child allowed
                pid[0] = pid[i];
                mpy = i;
            }
        }
    }

    if (!pid[0]) {        //child
        y = pow(2, mpy);  //different mpy value for each child
        printf(RESET "Child %d: task done, y = %d. Exiting.\n", getpid(), y);
        exit(y);
    }

    if (pid[0] > 0) {  //parent
        for (i = 0; i < N; i++) {
            waitpid(pid[i], &status, 0);
            printf(GRN "Parent (%d): collected data from child %d. 2^%d = %d\n", getpid(), pid[i], i, WEXITSTATUS(status));
            y += WEXITSTATUS(status);
        }
        printf(MAG "Parent: the result is %d.\n", y);
    }

    return 0;
}