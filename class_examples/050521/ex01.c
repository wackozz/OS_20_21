#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#define NUM_THREADS 5

struct thread_data {
  int thread_id;
  char message[100];
};

void *PrintHello(void *threadarg){
    struct thread_data *mydata;
    mydata = (struct thread_data *) threadarg;
    printf("Thread number %d, the message is %s",mydata->thread_id,mydata->message);
    pthread_exit(NULL);
}
int main() {
  struct thread_data thread_data_array[NUM_THREADS];
  pthread_t threads[NUM_THREADS];

  char messages[NUM_THREADS][100];
  strcpy(messages[0], "English: Hello World!\n");
  strcpy(messages[1], "French: Bounjour, le monde!\n");
  strcpy(messages[2], "Spanish: Hola al mundo!\n");
  strcpy(messages[3], "German: Guten Tag, Welt!\n");
  strcpy(messages[4], "Persian: Salam Donya!\n");

  for (int t = 0; t < NUM_THREADS; t++) {
    thread_data_array[t].thread_id = t;
    strcpy(thread_data_array[t].message, messages[t]);
    int rc = pthread_create(&threads[t], NULL, PrintHello, (void*)&thread_data_array[t]);
    if (rc != 0) {
      printf("Error in creating thread number %d\n", t);
      exit(-1);
    }
  }
  for (int t=0; t< NUM_THREADS; t++){
      pthread_join(threads[t],NULL);
      printf("Thread %lu terminated\n",threads[t]);
  }
  return 0;
}