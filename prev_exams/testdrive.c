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
char* product_bin = (char*)malloc(sizeof(char));
int* flag_product_bin = (int*)malloc(sizeof(int));

char* random_str() {
  // simple function to generate 11byte random strings
  char* str = (char*)malloc(11 * sizeof(char));
  for (int i = 0; i < 10; i++) {
    if (rand() % 10 > 8) {  // 2/10 is a number
      *(str + i) = (char)(rand() % 10 + 48);
    } else {
      *(str + i) = (char)(rand() % 26 + 97);
    }
  }
  str[10] = '\0';
  return str;
}

int hasDigits(char* str) {
  for (int i = 0; i < (int)strlen(str); i++) {
    if (isdigit(str[i])) {
      return 1;
    }
  }
  return 0;
}
/*
void* Producer(void* arg) {
  int count = 0;
  while (count < 10) {
    strcpy(product_bin + ind_pro * 11, random_str());
    printf("[%lu]Inserted %s in pos. %d\n", pthread_self(),
           (product_bin + ind_pro * 11), ind_pro);
    ind_pro++;
    count++;
  }
  pthread_exit(NULL);
}
*/
void* Verifier(void* arg) {
  int loc_ind_ver = 0;
  while (loc_ind_ver < 30) {
    loc_ind_ver = ind_ver;
    ind_ver++;
    if (hasDigits(product_bin + loc_ind_ver * 11)) {
      flag_product_bin[loc_ind_ver] = 0;
    } else {
      flag_product_bin[loc_ind_ver] = 1;
    }
    printf("[%lu]Checked %s in pos. %d: result %d \n", pthread_self(),
           (product_bin + loc_ind_ver * 11), loc_ind_ver,
           flag_product_bin[loc_ind_ver]);
  }
  printf("ok?\n");
  pthread_exit(NULL);
}

int main() {
  // dynamic allocation of variables
  pthread_t* tid = (pthread_t*)malloc(8 * sizeof(pthread_t));
  product_bin = (char*)realloc(product_bin, (N - 2) / 2 * 11 * sizeof(char));
  flag_product_bin = (int*)realloc(product_bin, (N - 2) / 2 * 10 * sizeof(int));
  // inizialization of rand() seed
  srand(time(NULL));
  random_str();
  // product threads
  /*
  for (int i = 0; i < 4; i++) {
    pthread_create(tid + i, NULL, Producer, (void*)product_bin);
  }
  sleep(1);
  */
 
  // verification threads
  for (int i = 4; i < 8; i++) {
    pthread_create(tid + i, NULL, Verifier, (void*)product_bin);
  }
  pthread_join(*(tid + 0), NULL);
  pthread_join(*(tid + 1), NULL);
  pthread_join(*(tid + 2), NULL);
  pthread_join(*(tid + 3), NULL);
  pthread_join(*(tid + 4), NULL);
  pthread_join(*(tid + 5), NULL);
  pthread_join(*(tid + 6), NULL);
  pthread_join(*(tid + 7), NULL);
}