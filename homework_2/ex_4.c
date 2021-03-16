#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

char * myGetTime(){
    time_t rawseconds;
    time(&rawseconds);
    char * time_str = asctime(localtime(&rawseconds));
    time_str[strlen(time_str)-1] = 0; //null for the \n
    return time_str;
}

int main()
{
    int pid = fork();
    int status;


    if (!pid) //child1
    {
        sleep(5);
        printf("%s\tCHILD1, exiting.\n",myGetTime());
        exit(1);
    }

    if (pid > 0) //parent
    {
        pid = fork();

        if (!pid) //child2
        {
            sleep(5);
            printf("%s:\tCHILD2, exiting.\n",myGetTime());
            exit(2);
        }
        else
        {
            printf("%s:\tPARENT here, waiting for child2 to exit...\n",myGetTime());
            waitpid(pid,&status,0);
            printf("%s:\tPARENT here, exiting.\n",myGetTime());
        }
    }
    return 0;
}