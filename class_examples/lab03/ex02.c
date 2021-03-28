//we want to execute a child that has the same binary of the helloWorld program

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    pid = fork();

    if (!pid) {
        printf("I'm child and I'm calling the helloworld");
        execl("/home/wackoz/github/OS_20_21/class_examples/lab03/helloWorld", "helloWorld", "/home/wackoz/github/OS_20_21/class_examples/lab03", NULL);
        printf("Failed Execl\n");
        exit(-1);
    }

    else {
        wait(NULL);
        printf("Child terminated\n");
    }
    return 0;
}