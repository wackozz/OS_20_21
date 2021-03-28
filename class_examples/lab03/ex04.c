//system example  2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


int main(){

    int sys_value;
    char command[50];
    strcpy(command, "ls -a");
    printf("Calling system()\n");
    sys_value = system(command);
    printf("System() finished, now calling execute()\n");
    execl("/home/wackoz/github/OS_20_21/class_examples/lab03/helloWorld", "helloWorld", "/home/wackoz/github/OS_20_21/class_examples/lab03", NULL);
    //we should not see after this:
    printf("FInished with execl()\n");
    exit(0);
}