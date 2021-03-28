#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
char command[128]= "find ";

    if (argc != 2) {
        printf("Error. Wrong argument number.\n");
    }

    else {
        system(strcat(command,argv[1]));
    }

    return 0;
}