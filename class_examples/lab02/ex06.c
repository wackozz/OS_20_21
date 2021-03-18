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
    printf("Child 1 with pid %d\n", getpid());
    sleep(3);
    printf("I am child1 with pid of %d and I want to terminate\n", getpid());
    exit(0);
  }
  else
  { //parent proc.
    pid = fork();
    if (!pid)
    {
      printf("Child2 with pid of %d\n", getpid());
      sleep(8);
      printf("I am child2 with pid of %d and i want to terminate\n", getpid());
      exit(1);
    }
    else
    {
      pid1 = 1;
      while (pid1 > 0)
      {
        pid1 = wait(&status);
        if (pid1 > 0)
        {
          printf("Child with pid %d is terminated with status %d\n", pid1, status);
        }
      }
    }
  }
  return 0;
}
