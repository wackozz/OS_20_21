#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h> //system calls

int x = 5;

int main()
{
  pid_t pid, pid1;
  int status;

  printf("I am the original program with pid %d\n", getpid());

  pid = fork();

  if (pid == 0)
  { //child
    x += 15;
    printf("I am the child with pid %d and value is %d\n", getpid(), x);
    exit(0);
  }
  else
  { //parent
    pid1 = wait(&status);

    printf("The child that terminated is with pid %d\n", pid1);
    printf("I am the parent and the value is = %d\n", x);
  }
  return 0;
}
