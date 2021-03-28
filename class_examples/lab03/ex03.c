//system example

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char command[50];
    int sys_value;

    strcpy(command, "ls -a");
    sys_value = system(command);
    printf("Return from system(): %d \n", sys_value);
    exit(0);
}