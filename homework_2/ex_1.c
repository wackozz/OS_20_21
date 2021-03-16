#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int pid = fork();

    if(!pid){ //child
         printf("   --CHILD\tpid=%d\tChild pid =%d\n",getpid(),pid);
    }
    else if (pid>0){
         printf("PARENT\tpid=%d\tChild pid =%d\n",getpid(),pid);
    }
}