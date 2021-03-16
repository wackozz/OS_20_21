#define N 6
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

char *myGetTime()
{
    time_t rawseconds;
    time(&rawseconds);
    char *time_str = asctime(localtime(&rawseconds));
    time_str[strlen(time_str) - 1] = 0; //null for the \n
    return time_str;
}

void fiftyRandomNumbers(int child_id)
{
    printf("[%s] CHILD (%d), pid=%d ", myGetTime(), child_id, getpid());
    for (int k = 0; k < 49; k++)
    {
        printf("%.1f, ", pow(random() % 10, 2));
    }
    printf("%.1f.\n\n", pow(random()%10, 2));
}
int main()
{
    int child_id;
    int i, j, timeout;
    timeout = 3;
    i = 1;
    j = 1;
    int pid[6] = {0};
    pid[0] = fork();
    if (pid[0] > 0) //if parent
    {
        child_id = -1;
        printf("[%s] PARENT (pid=%d): forked CHILD %d\n\n", myGetTime(), getpid(), pid[0]);
    }
    else
    {
        child_id = 0;
        printf("[%s] CHILD (pid=%d): my child ID is #%d\n\n", myGetTime(), getpid(), child_id);
    }
    for (i = 1; i < N; i++)
    {
        if (pid[i - 1] > 0) //if parent
        {
            pid[i] = fork();
            if (pid[i] > 0)
            {
                printf("[%s] PARENT (pid=%d): forked CHILD %d\n\n", myGetTime(), getpid(), pid[i]);
                sleep(1);
            }
            else
            {
                child_id = i;
                printf("[%s] CHILD (pid=%d): my child ID is #%d\n\n", myGetTime(), getpid(), child_id);
            }
        }
    }

    while (1)
    {
        if (child_id > -1 && child_id < 4)
        {
            printf("[%s] CHILD (%d): %ld\n\n", myGetTime(), child_id, random() % 10);
        }
        else if(child_id > -1)
        {
            fiftyRandomNumbers(child_id);
        }
        sleep(1);
        timeout--;
    }
}