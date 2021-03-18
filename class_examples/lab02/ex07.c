#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int i, status, N = 3;
    pid_t pid[N];

    for (i = 0; i < N; i++)
    {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            sleep(1);
            exit(10 + i);
        }
    }

    for (i = 0; i < N; i++)
    {
        waitpid(pid[i], &status, 0);
        if (WIFEXITED(status))
            printf("The child with pid of %d was terminated, the exit status is %d \n", pid[i], WEXITSTATUS(status));
        else
            printf("The child with pid %d did not terminate correctly \n", pid[i]);
    }
    return 0;
}