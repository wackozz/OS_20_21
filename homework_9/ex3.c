#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  int var;
  sem_t sem;
} shared_data;

int main() {
  srand(time(NULL));
  shared_data *sh_data;
  int shmid;
  key_t key = ftok("data.dat", 'A');
  if ((shmid = shmget(key, sizeof(shared_data), IPC_CREAT | 0666)) < 0) {
    perror("shmget failed");
    exit(EXIT_FAILURE);
  }

  if ((sh_data = (shared_data *)shmat(shmid, NULL, 0)) < 0) {
    perror("shmget failed");
    exit(EXIT_FAILURE);
  }
  sem_init(&sh_data->sem, 1, 1);
  int pid = fork();
  int pid2, R, R2;
  if (pid) {  // parent
    while (1) {
      R = rand() % 10;
      sleep(R);
      sem_wait(&sh_data->sem);
      printf("Parent: putting R=%d\n",R);
      sh_data->var = R;
      sem_post(&sh_data->sem);
    }
  } else {  // child
    while (1) {
      R = rand() % 3;
      sleep(R);
      printf("Son: forking\n");
      pid2 = fork();
      if (!pid2) {  // nephew
        while (1) {
          R2 = rand() % 5;
          sleep(R);
          sem_wait(&sh_data->sem);
          if (sh_data->var == -1) {
             //printf("Nephew (%d): sh_data = -1\n", getppid());
             sem_post(&sh_data->sem);
          } else {
            printf("Nephew (%d): SV: %d, exiting\n",getppid(), sh_data->var);
            sh_data->var = -1;
            sem_post(&sh_data->sem);
            exit(EXIT_SUCCESS);
          }
        }
      }
    }
  }
  sem_destroy(&sh_data->sem);
  shmdt(sh_data);
  shmctl(shmid, IPC_RMID, 0);
}