#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int hasDigits(char* str) {
  for (int i = 0; i < (int)strlen(str); i++) {
    if (isdigit(str[i])) {
      return 1;
    }
  }
  return 0;
}

int main(){
    printf("%d\n",hasDigits("1djen"));
}