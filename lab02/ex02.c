#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> //system calls

int main(){
  pid_t fork_result, pid, parentpid;

  fork_result = fork();
  
  if (fork_result < 0){
    printf("Fork failed\n");
  }
  else if (fork_result == 0){ //child
    pid = getpid();
    parentpid = getppid();
    printf("I am the child process and the fork result is %d. My pid is %d and my parent pid is %d\n", fork_result, pid, parentpid);
  }
  else{ //parent
    pid = getpid();
    printf("I am the parent process and the fork result is %d. My pid is %d. \n", fork_result, pid);
  }
}


