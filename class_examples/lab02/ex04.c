#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h> //system calls

int main(){
  pid_t pid;

  pid = fork();
  
  if (pid < 0){
    printf("Fork failed\n");
  }
  else if (pid == 0){ //child
    printf("1: I am the child process and the fork result is %d. My pid is %d and my parent pid is %d\n", getpid(),pid,  getppid());
    exit(1);
    printf("2: I am the child process and the fork result is %d. My pid is %d and my parent pid is %d\n", getpid(), pid, getppid());
  }
  else{ //parent
    pid = getpid();
    printf("I am the parent process and the fork result is %d. My pid is %d. \n", getpid(), pid);
  }
}


