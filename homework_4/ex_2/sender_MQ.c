#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#define NAME "/myshmem"
#define SIZE 512

void title() {
  printf("\e[1m     ______________________________________\n");
  printf("\e[1m   /\t\t\t\t\t   /\n");
  printf("\e[1m  /   Homework 04 - Exercize no. 2,TX     /\n");
  printf("\e[1m /_______________________________________/\n\n");
}
void whoAmI() {
  printf(CYN "[Process %d, child of %d.]:", getpid(), getppid());
  printf(RESET);
}

struct myMsgBuf {
  long mtype;
  char mtext[SIZE];
};

int main() {
  int msgid, running = 1;
  struct myMsgBuf buffer;
  key_t key = ftok("key.dat", 'B');
  if (key == -1) {
    perror("ftok: ");
    exit(-1);
  }
  title();
  system("touch key.dat");

  if ((msgid = msgget(key, IPC_CREAT | 0666)) == -1) { //open queue
    perror("msgid: ");
    exit(-1);
  }
  buffer.mtype = 1;
  while (running) {
    printf("Enter command:");
    //scanf("%s", buffer.mtext);
    fgets(buffer.mtext, sizeof(buffer.mtext), stdin);
    buffer.mtext[SIZE-1] = '\0';
    if ((msgsnd(msgid, &buffer, strlen(buffer.mtext)+1, 0)) == -1) {
      perror("msgsnd: ");
      exit(-1);
    }
    if (strcmp("exit", buffer.mtext) == 0) {  // loop exit
      running = 0;
    }
  }

  return 0;
}