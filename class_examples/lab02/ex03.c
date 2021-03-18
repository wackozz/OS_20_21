#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> //system calls

int main(){
  pid_t pid;

  pid = fork();
  
  if (pid < 0){
    printf("Fork failed\n");
  }
  else if (pid == 0){ //child
    printf("I am the child process and the fork result is %d. My pid is %d and my parent pid is %d\n", getpid(),pid,  getppid());
    sleep (4);
    printf("I am the child process and the fork result is %d. My pid is %d and my parent pid is %d\n", getpid(), pid, getppid());
  }
  else{ //parent
    pid = getpid();
    printf("I am the parent process and the fork result is %d. My pid is %d. \n", getpid(), pid);
  }


