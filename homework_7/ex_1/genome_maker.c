#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 50000

char randomchar() {
  int num = rand() % 4;
  switch (num) {
    case 0:
      return 'C';
      break;
    case 1:
      return 'G';
      break;
    case 2:
      return 'A';
      break;

    case 3:
      return 'T';
      break;

    default:
      return 'A';
      break;
  }
}

int main() {
  srand(time(NULL));
  FILE *fp;
  if ((fp = fopen("genome.dat", "w+")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < N; i++) {
    fprintf(fp, "%c", randomchar());
  }
  fclose(fp);
}