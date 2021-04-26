#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signo){
    printf("you passed ctrl+c");
    exit(0);
}

int main(){
    signal(SIGINT, signal_handler);

    while(1){
        printf("Hello World\n");
    }
}