#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N 10
int ind_pro = 0;
int ind_ver = 0;
const char* filename = "data.dat";
pthread_mutex_t m_pro;
pthread_mutex_t m_ver;

void random_str(char* str) {
  // simple function to generate 11byte random strings
  for (int i = 0; i < 10; i++) {
    if (rand() % 10 > 8) {  // 2/10 is a number
      *(str + i) = (char)(rand() % 10 + 48);
    } else {
      *(str + i) = (char)(rand() % 26 + 97);
    }
  }
  str[10] = '\0';
}

int hasDigits(char* str) {
  for (int i = 0; i < (int)strlen(str); i++) {
    if (isdigit(str[i])) {
      return 1;
    }
  }
  return 0;
}

void* Producer(void* arg) {
  char str[11] = {0};
  char* product_bin = (char*)arg;
  int count = 0;

  pthread_mutex_lock(&m_pro);
  while (count < 10) {
    random_str(str);
    memcpy(product_bin + ind_pro * 11, str,strlen(str)+1);
    printf("[%lu]Inserted %s in pos. %d\n", pthread_self(),
           (product_bin + ind_pro * 11), ind_pro);
    ind_pro++;
    count++;
  }
  pthread_mutex_unlock(&m_pro);

  pthread_exit(NULL);
}
/*
void* Verifier(void* arg) {
  int loc_ind_ver = 0;
  while (loc_ind_ver < 30) {
    pthread_mutex_lock(&m_ver);
    loc_ind_ver = ind_ver;
    ind_ver++;
    pthread_mutex_unlock(&m_ver);
    if (hasDigits(product_bin + loc_ind_ver * 11)) {
      flag_product_bin[loc_ind_ver] = 0;
    } else {
      flag_product_bin[loc_ind_ver] = 1;
    }
    printf("[%lu]Checked %s in pos. %d: result %d \n", pthread_self(),
           (product_bin + loc_ind_ver * 11), loc_ind_ver,
           flag_product_bin[loc_ind_ver]);
  }
  pthread_exit(NULL);
}
*/
void* WasteManager(void* arg) { pthread_exit(NULL); }
void* StorageFacility(void* arg) { pthread_exit(NULL); }

int main() {
  // mutex init
  pthread_mutex_init(&m_pro, NULL);
  pthread_mutex_init(&m_ver, NULL);
  // dynamic allocation of variables
  pthread_t* tid = (pthread_t*)malloc(N * sizeof(pthread_t));
  char* product_bin = (char*)calloc((N - 2) / 2 * 11 * 10, sizeof(char));
  int* flag_product_bin = (int*)calloc((N - 2) / 2 * 10, sizeof(int));
  // inizialization of rand() seed
  srand('a');
  // product threads
  for (int i = 0; i < ((N - 2) / 2); i++) {
    pthread_create(tid + i, NULL, Producer, (void*)product_bin);
  }
    for (int i = 0; i < N; i++) {
    pthread_join(*(tid + i), NULL);
  }

  FILE* fp;
  fp = fopen("data.dat", "w+");
  for (int i = 0; i < 40; i++) {
    fprintf(fp, "%s\n", (product_bin + i * 11));
    fprintf(stdout, "%s\n", (product_bin + i * 11));
  }

  fclose(fp);

  free(tid);
  free(product_bin);
  free(flag_product_bin);
  pthread_mutex_destroy(&m_pro);
  pthread_mutex_destroy(&m_ver);
}