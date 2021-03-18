#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> //system calls

int main(){
  pid_t fork_result;

  fork_result = fork();
  
  if (fork_result < 0){
    printf("Fork failed\n");
  }
  else if (fork_result == 0){
    printf("I am the child process and the fork result is %d \n", fork_result);
  }
  else{
    printf("I am the parent process and the fork result is %d \n", fork_result);
  }
}

