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
  char buffer[MAX_TEXT];
  int running = 1;
  int len, msgsnd_ret;
  // KEY CREATION
  system("touch msg.txt");
  key_t key = ftok("msg.txt", 'B');
  if (key == -1) {
    perror("ftok(): ");
  }
  // Create a new message queue
  int msgid = msgget(key, IPC_CREAT | 0666);
  if (msgid == -1) {
    perror("msgget:");
  }
  some_data.my_msg_type = 1;
  while (running) {
    printf("Enter some text:\n");
    scanf("%s", buffer);
    printf("You entered: %s\n", buffer);
    strcpy(some_data.some_text, buffer);
    len = strlen(some_data.some_text) + 1;  //+1 accounts for string terminator
    msgsnd_ret = msgsnd(msgid, &some_data, len, 0);
    if (msgsnd_ret == -1) {
      perror("msgsnd: ");
      exit(errno);
    }
    if (strcmp(buffer, "end") == 0) {
      running = 0;
    }
  }
}