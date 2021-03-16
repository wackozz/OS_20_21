#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pid = fork();

    {

        if (!pid) //child1
        {
            pid = fork();
            if (!pid) //child2
            {
                pid = fork();
            }
        }
    }

    printf("Process %d,\tParent pid =%d\n", getpid(), getppid());
    return 0;
}