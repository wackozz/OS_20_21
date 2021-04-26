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

#define MAX_TEXT 512

struct my_msg_st {
  long int my_msg_type;
  char some_text[MAX_TEXT];
};

// sender and receiver message queue programs
int main() {
  struct my_msg_st some_data;
  int running = 1;
  key_t key = ftok("msg.txt", 'B');
  if (key == -1) {
    perror("ftok(): ");
  }
  // Create a new message queue
  int msgid = msgget(key, IPC_CREAT | 0666);
  if (msgid == -1) {
    perror("msgget:");
  }

  while (running) {
    if ((msgrcv(msgid, &some_data, MAX_TEXT, 0, 0)) == -1) {
      perror("msgrcv: ");
      exit(EXIT_FAILURE);
    }
    printf("You wrote: %s\n", some_data.some_text);
    if (strcmp(some_data.some_text, "end") == 0) {
      running = 0;
    }
  }
  if (msgctl(msgid, IPC_RMID, 0) == -1) {
    perror("Error in msgctl: ");
  }
}