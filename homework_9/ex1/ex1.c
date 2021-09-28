#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

sem_t sem;

void* print_msg(void * arg){
sem_wait(&sem);
printf("Hello, I'm thread no. 2 (%lu)\n\n",pthread_self());
sem_post(&sem);
sleep(1);
pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < 10; i++) {
    sem_init(&sem,0,1);
    pthread_t thread_id;
    pthread_create(&thread_id,NULL,print_msg,NULL);
    sem_wait(&sem);
    printf("Hello, I'm thread no. 1 (%lu)\n",pthread_self());
    sem_post(&sem);
    pthread_join(thread_id, NULL);
    }
    sem_destroy(&sem);
     return 0; }