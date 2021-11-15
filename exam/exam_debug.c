#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_FILENAME 100

// global variables
int N;
int prod_cnt = 0;
int ver_cnt = 0;
pthread_mutex_t mutex;
pthread_cond_t cond1, cond2;
char filename[MAX_FILENAME];

// struct
typedef struct {
  char product[11];
  int error;
  int prod;
} data;

// functions
void randomStr(char *str) {
  // strlen check to avoid segfault
  if (strlen(str) != 10) {
    strcpy(str, "\0");
  } else {
    int prob = 0;
    // 1/8 char generated is a digit (error)
    for (int i = 0; i < 10; i++) {
      prob = rand() % 8;
      if (prob == 0) {
        str[i] = (char)(rand() % 10 + '0');  // from 0 to 9
      } else {
        str[i] = (char)(rand() % 26 + 'a');  // from a to z,
      }
    }
    str[11] = '\0';
  }
}

int checkString(char *str) {
  int error = 0;
  if (strlen(str) != 10) {
    error = -1;
  } else {
    for (int i = 0; i < 10; i++) {
      if (isdigit(str[i])) {
        error = 1;
        break;
      }
    }
  }
  return error;
}

int howManyOkChar(char *str) {
  if (strlen(str) != 10) {
    return -1;
  } else {
    int error = 0;
    for (int i = 0; i < 10; i++) {
      if (isdigit(str[i])) {
        error++;
      }
    }
    return 10 - error;
  }
}

// thread functions
void *producer(void *arg) {
  data *bin = (data *)arg;
  // creating a 10 char str
  char str[11] = "0000000000";

  for (int i = 0; i < 10; i++) {
    pthread_mutex_lock(&mutex);
    // random string generation
    randomStr(str);
    // copy the generated string into bin
    strcpy(bin[prod_cnt].product, str);
    // stdout output
    printf("\x1b[36mProducer(%lu): making product no. %d [%s]\x1b[0m\n",
           pthread_self(), prod_cnt, bin[prod_cnt].product);
    // flag prod active (to communicate with verifier)
    bin[prod_cnt].prod = 1;
    prod_cnt++;
    pthread_mutex_unlock(&mutex);
    // wake-up all the waiting verifiers
    pthread_cond_broadcast(&cond1);
    // sleep
    sleep(rand() % (N / 2));
  }

  pthread_exit(NULL);
}

void *verifier(void *arg) {
  data *bin = (data *)arg;
  // cycle through each allocated data struct
  while (ver_cnt < 10 * ((N - 2) / 2)) {
    pthread_mutex_lock(&mutex);
    // check if the i-th product has been made by producer.
    while (bin[ver_cnt].prod == 0) {
      printf("(%lu) waiting...\n", pthread_self());
      // wait for the producer to make a new product
      pthread_cond_wait(&cond1, &mutex);
    }
    // check that ver_cnt is less than total allocated data struct
    //(threads resume from this point)
    if (ver_cnt < 10 * ((N - 2) / 2)) {
      //stdout print for debugging
      printf("\x1b[31mVerifier(%lu): checking product no. %d [%s]\x1b[0m\n",
             pthread_self(), ver_cnt, bin[ver_cnt].product);
      // check error thour checkString function
      bin[ver_cnt].error = checkString(bin[ver_cnt].product);
      ver_cnt++;
    }
    // wake-up waiting storageFacility and wasteManagement
    pthread_cond_broadcast(&cond2);
    pthread_mutex_unlock(&mutex);
    // sleep
    sleep(rand() % (N / 2) + 1);
  }
  pthread_exit(NULL);
}

void *storageFacility(void *arg) {
  int i = 0;
  data *bin = (data *)arg;
  FILE *fp;
  fp = fopen(filename, "w+");  // create file, overwrite it if it exists
  fclose(fp);
  // cycle through each allocated data struct
  while (i < 10 * ((N - 2) / 2)) {
    pthread_mutex_lock(&mutex);
    // wait if index is above the verifier one -1 to avoid sync problems
    while (i > ver_cnt - 1) {
      printf("StorageFacility: waiting..\n");
      pthread_cond_wait(&cond2, &mutex);
    }
    // check if the product is not flagged by the verifier
    if (bin[i].error == 0) {
      printf(
          "\x1b[33mStorageFacility: product %d contains no error, writing "
          "%s..\x1b[0m\n",
          i, bin[i].product);
      // file opening check
      if (!fopen(filename, "a")) {
        perror("ERROR IN OPENING FILE:");
        pthread_exit(NULL);
      };
      // print ok product to fp
      fprintf(fp, "%s\n", bin[i].product);
      fclose(fp);
    }
    i++;
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

void *wasteManagement(void *arg) {
  data *bin = (data *)arg;
  int i = 0;
  // cycle through each allocated data struct
  while (i < 10 * ((N - 2) / 2)) {
    pthread_mutex_lock(&mutex);
    // wait if index is above the verifier one -1 to avoid sync problems
    while (i > ver_cnt - 1) {
      printf("wasteManager: waiting..\n");
      pthread_cond_wait(&cond2, &mutex);
    }
    // check if the product if flagged by the verifier
    if (bin[i].error == 1) {
      printf(
          "\x1b[35mWasteManagement: product no. %d [%s], tot. %d char "
          "ok\n\x1b[0m",
          i, bin[i].product, howManyOkChar(bin[i].product));
    }
    i++;
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  // srand init
  srand(time(NULL));
  // check argc
  if (argc < 3) {
    printf("ERROR: wrong argument number. Usage: %s N filename", argv[0]);
    exit(EXIT_FAILURE);
  }
  // static variables
  N = atoi(argv[1]);
  // verify if N is even or odd, if odd N rounded to the next even
  if (N % 2) {
    N++;
  }
  strcpy(filename, argv[2]);  // copy filename into global variable
  // dynamic memory alloc
  data *arg =
      (data *)malloc(((N - 2) / 2) * 10 * sizeof(data));  // 10*numofproducer
  pthread_t *tid = (pthread_t *)malloc(N * sizeof(pthread_t));
  // initializations
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond1, NULL);
  pthread_cond_init(&cond2, NULL);
  for (int i = 0; i < ((N - 2) / 2) * 10; i++) {
    arg[i].error = 0;
    arg[i].prod = 0;
  }
  // producer creation
  for (int i = 0; i < (N - 2) / 2; i++) {
    pthread_create(tid + i, NULL, producer, (void *)arg);
  }
  // verifier creation
  for (int i = (N - 2) / 2; i < N - 1; i++) {
    pthread_create(tid + i, NULL, verifier, (void *)arg);
  }
  // storageFacility creation
  pthread_create(tid + (N - 1), NULL, storageFacility, (void *)arg);
  // wasteManagement creation
  pthread_create(tid + N, NULL, wasteManagement, (void *)arg);

  // thread join
  for (int i = 0; i < N; i++) {
    pthread_join(*(tid + i), NULL);
  }
  // free dynamic mem, destroy
  free(arg);
  free(tid);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond1);
  pthread_cond_destroy(&cond2);
  return 0;
}