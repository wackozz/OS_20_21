#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

sem_t *sem;

void fun1(int *shm) {
  while (1) {
    int R = rand() % 10+1;
    printf("[%d] Parent waiting %d seconds..\n",getpid(),R);
    sleep(R);
    sem_wait(sem);
    printf("[%d] Parent handling shm..\n",getpid());
    *shm = R;
    sem_post(sem);
  }
}

void fun3(int *shm) {
    int R = rand()%5+1;
    sleep(R);
    printf("[%d] Nephew waiting %d seconds.. \n",getpid(),R);
    sem_wait(sem);
    if(*shm == -1){
        printf("[%d] Nephew handling shm.. \n",getpid());
        sem_post(sem);
    }
    else{
        printf("[%d] Nephew handling shm; current value = %d\n",getpid(),*shm);
        *shm = -1;
        sem_post(sem);
        printf("[%d] Nephew EXITING. \n",getpid());
        exit(-1);
    }

}

void fun2(int *shm) {
  while (1) {
    int R = rand() % 3+1;
    printf("[%d] Child waiting %d seconds..\n",getpid(),R);
    sleep(R);
    pid_t pid = fork();
    if (pid == 0) {//child
        fun3(shm);
    }
  }
}



int main() {
  srand(time(NULL));
  key_t key = ftok(".", 'x');
  key_t key2 = ftok(".", 'y');
  int *shm;
  int shmid, shmid_sem;
  // creation, ataching of the SV
  if ((shmid = (shmget(key, sizeof(int), IPC_CREAT | 0666))) < 0) {
    perror("shmget():");
    exit(-1);
  }
  if ((shm = (int *)shmat(shmid, NULL, 0)) < 0) {
    perror("shmat():");
    exit(-1);
  }
  *shm = -1;

  if ((shmid_sem = (shmget(key2, sizeof(sem_t), IPC_CREAT | 0666))) < 0) {
    perror("shmget()");
    exit(-1);
  }
  if ((sem = (sem_t *)shmat(shmid_sem, NULL, 0)) < 0) {
    perror("shmat()");
    exit(-1);
  }
  sem_init(sem, 1, 1);
  pid_t pid;
  pid = fork();
  if (pid == 0) {  // child
    fun2(shm);
  }
  if (pid != 0) {  // parent
    fun1(shm);
  }

  // detaching, deleting shm
  sem_destroy(sem);
  shmdt(sem);
  shmdt(shm);
  shmctl(shmid, IPC_RMID, 0);
  shmctl(shmid_sem, IPC_RMID, 0);

  return 0;
}