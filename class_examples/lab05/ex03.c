#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void signal_handler(int signo) {
  printf("My child killed me.\n");
  exit(0);
}

int main() {
  pid_t child_pid, parent_pid;

  child_pid = fork();

  if (child_pid == 0) {  // child
    parent_pid = getppid();
    printf(
        "I am the child with the pid: %d and I want to send the signal to my "
        "parent with the pid: %d\n",
        getpid(), parent_pid);
    kill(parent_pid, SIGINT);
  } else {
    signal(SIGINT, signal_handler);
    printf("I am the parent with pid of %d, I will loop forever\n", getpid());
    while (1) {
    }
  }
}