#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pid = fork();

    {

        if (pid > 0) //parent
        {
            pid = fork();
            if (pid > 0)
            {
                pid = fork();
            }
        }
    }

    printf("Process %d,\tParent pid =%d\n", getpid(), getppid());
    return 0;
}