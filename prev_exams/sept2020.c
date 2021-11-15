#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct {
  char product[10];
  int error;
  int p;
} data;

// global variables
int N;
int ind = 0, ind_ver = 0;
sem_t sem[2];
pthread_mutex_t mutex_prod, mutex_ver;

/*--------------------
FUNCTIONS DEFINITIONS
--------------------*/

int hasNum(char *str);

char *newProduct(char *str);

void *producer(void *arg);

void *verifier(void *arg);

void *storage_facility(void *arg) { pthread_exit(NULL); }

void *waste_management(void *arg) { pthread_exit(NULL); }

/*--------------------
--------MAIN----------
--------------------*/

int main(int argc, char *argv[]) {
  // check argc
  if (argc != 3) {
    printf("Wrong argument number. Usage: %s N filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // init srand, mutex
  srand(time(NULL));
  sem_init(&sem[0], 0, 1);
  sem_init(&sem[1], 0, 0);
  pthread_mutex_init(&mutex_prod, NULL);
  pthread_mutex_init(&mutex_ver, NULL);

  // static variables
  N = atoi(argv[1]);

  // dynamic variables
  pthread_t *tid = (pthread_t *)malloc(N * sizeof(pthread_t));
  data *bin = (data *)malloc(N * 10 * sizeof(data));

  // thread generation: producers

  for (int i = 0; i < ((N - 2) / 2); i++) {
    pthread_create(tid + i, NULL, producer, bin);
  }

  // thread generation: consumers
  for (int i = ((N - 2) / 2); i < N - 2; i++) {
    pthread_create(tid + i, NULL, verifier, bin);
  }

  // thread generation:
  pthread_create(tid + (N - 1), NULL, storage_facility, NULL);

  // thread generation:
  pthread_create(tid + N, NULL, waste_management, NULL);

  // thread join
  for (int i = 0; i < N; i++) {
    pthread_join(*(tid + i), NULL);
  }

  // mutex destroy
  pthread_mutex_destroy(&mutex_prod);
  pthread_mutex_destroy(&mutex_ver);
  sem_destroy(&sem[0]);
  sem_destroy(&sem[1]);
  // free memory
  free(bin);
  free(tid);
}

/*--------------------
FUNCTIONS DECLARATIONS
--------------------*/

char *newProduct(char *str) {
  int dice;
  for (int i = 0; i < 10; i++) {
    if (rand() % 12 < 2)
      str[i] = (char)(rand() % 9) + 48;  // random num
    else
      str[i] = (char)(rand() % 26) + 97;  // random letter
  }
  str[10] = '\0';
  return str;
}

int hasNum(char *str) {
  /* check if string passed as argument contains a digit or not. Returns 1 if
   * true, 0 if false */
  int result = 0;  // false
  for (int i = 0; i < strlen(str); i++) {
    if (isdigit(str[i])) {
      result = 1;
      break;
    }
  }
  return result;
}

void *producer(void *arg) {
  data *bin = arg;
  char str[11];
  printf("[Producer %lu] created\n", pthread_self());
  for (int i = 0; i < 10; i++) {
    sem_wait(&sem[0]);
    pthread_mutex_lock(&mutex_prod);
    newProduct(str);
    printf("[Producer %lu] written %s in bin (slot %d)\n", pthread_self(), str,
           ind);
    strcpy(bin[ind].product, str);
    bin[ind].p = 1;
    ind++;
    pthread_mutex_unlock(&mutex_prod);
    sem_post(&sem[1]);
    // sleep(rand() % (N / 2)) + 1;
  }
  printf("[Producer %lu] exiting.\n", pthread_self());
  pthread_exit(NULL);

void *verifier(void *arg) {
  data *bin = arg;
  char str[11];
  printf("[Verifier %lu] created\n", pthread_self());
  for (int i = 0; i < 10; i++) {
    sem_wait(&sem[1]);
    pthread_mutex_lock(&mutex_ver);
    if (hasNum(bin[ind_ver].product)) {
      bin[ind_ver].error = 1;
      printf("[Verifier %lu] %s : ERROR. (slot %d)\n", pthread_self(),
             bin[ind_ver].product,ind_ver);
    } else {
      printf("[Verifier %lu] %s : OK. (slot %d)\n", pthread_self(), bin[ind_ver].product, ind_ver);
      bin[ind_ver].error = 0;
    }
    ind_ver++;
    pthread_mutex_unlock(&mutex_ver);
    sem_post(&sem[0]);
  }

  pthread_exit(NULL);
}