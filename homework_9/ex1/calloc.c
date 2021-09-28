#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
void fill_array(int n, int *array) {
  for (int i = 0; i < n; i++) {
    *(array + i) = rand() % 10;
  }
}

void print_array(int n, int *array) {
  for (int i = 0; i < n; i++) {
    printf("%d. %d\n", i+1, *(array + i));
  }
}

int main() {
  int n;
  printf("Length of the array:\n");
  scanf("%d", &n);
  int *a;
  a = (int*)malloc(sizeof(int) * n);
  fill_array(n,a);
  print_array(n,a);
  free(a);
  return 0;
}