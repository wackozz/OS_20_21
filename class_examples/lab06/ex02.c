#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void printids(const char *s) {
  pthread_t tid;
  pid_t pid;

  pid = getpid();
  tid = pthread_self();
  printf("The calling thread is: %s\n\t Pid = %d. Tid = %ld. \n", s, pid, tid);
} 

void *thread_func(void *p) {
  printids("NEW thread");
  pthread_exit(NULL);
}

int main() {
  pthread_t tid;  // thread id
  int err;
  err = pthread_create(&tid, NULL, thread_func, NULL);  // thread creation
  if (err != 0) {
    perror("pthread_create:");
  }

  printids("Main thread");
  sleep(3);
  return 0;
}