#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void alarm_handler(int a){
    printf("The alarm of 3 second is over\n");
}
int main(){
    signal(SIGALRM, alarm_handler);
    alarm(3);
    while(1){

    }
}