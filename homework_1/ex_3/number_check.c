#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
  if(argc!=2){
    printf("Error: wrong argument. Usage: number N\n");
  }
  else{
    if((atoi(argv[1])%2!=0)){
      printf("The number is odd.\n");
    }
    else{
      printf("The number is even.\n");
    }
  }
  return 0;
}

  
