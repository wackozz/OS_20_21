//we want to execute a child that has the same binary of the ls command

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    pid = fork();

    if (!pid) {
        execl("/bin/ls", "ls", "-a", "/home/wackoz/github/OS_20_21/class_examples", NULL);
        printf("Failed Execl\n");
        exit(-1);
    }

    else {
        wait(NULL);
        printf("Child Finished\n");
    }
    return 0;
}