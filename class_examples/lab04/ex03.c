#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define WRITE 1
#define READ 0

char const *phrase = "This is a message.";

int main() {
  int fd[2];
  pid_t pid;
  char message[25];

  pipe(fd);
  pid = fork();

  if (pid == 0) {  // child
    int num = 3;
    write(fd[WRITE], phrase, 18);
    write(fd[WRITE], &num, sizeof(int));
    sleep(5);
    close(fd[WRITE]);
  } else {  // parent
    int number;
    read(fd[READ], message, 18);
    printf("Reading a string from pipe: %s\n", message);
    read(fd[READ], &number, sizeof(int));
    printf("Reading an int from pipe: %d\n", number);
    close(fd[READ]);
  }
}