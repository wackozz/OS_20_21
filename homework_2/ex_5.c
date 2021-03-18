/*
v1.0
17 March 2021
coded by wackoz (s279895)

- random number output restricted between one and ten.
*/

#define N 6
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

void fiftyRandomNumbers(int child_id) {
    printf(MAG "[%s] CHILD (%d, pid = %d)= ", myGetTime(), child_id, getpid());
    for (int k = 0; k < 49; k++) {
        printf("%d, ", (int)pow(random() % 10, 2));
    }
    printf("%d.\n\n", (int)pow(random() % 10, 2));
}

int main() {
    int child_id;
    int i, j, timeout;
    timeout = 3;
    i = 1;
    j = 1;
    int pid[6] = {0};
    pid[0] = 1;
    for (i = 0; i < N; i++) {
        if (pid[0] > 0) {  //parent allowed
            pid[i] = fork();
            if (pid[i] > 0) {  //parent allowed
                printf(GRN "PARENT (%d): forked CHILD %d\n\n", getpid(), pid[i]);
                sleep(1);
            } else {  //child allowed
                pid[0] = pid[i];
                child_id = i;
                printf(RESET "CHILD (%d): my child ID is #%d\n\n", getpid(), child_id);
            }
        }
    }

    while (1) {
        if (child_id > -1 && child_id < 3) {
            printf(CYN "[%s] CHILD (%d, pid= %d): %ld\n\n", myGetTime(), child_id, getpid(), random() % 10);
            sleep(1);
        } else if (child_id > -1) {
            fiftyRandomNumbers(child_id);
            sleep(1);
        }
        sleep(1);
    }
}