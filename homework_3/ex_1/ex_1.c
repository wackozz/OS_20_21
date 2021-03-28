#define N 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

void title() {
    printf("\e[1m     ______________________________________\n");
    printf("\e[1m   /\t\t\t\t\t   /\n");
    printf("\e[1m  /\t Homework 03 - Exercize no. 1     /\n");
    printf("\e[1m /_______________________________________/\n\n");
}

void whoAmI() {
    printf(CYN "[Process %d, child of %d.]: ", getpid(), getppid());
    printf(RESET);
}

void createProcesses(int proc_number, int *id) {
    pid_t pid = 1;
    *id = 0;  //parent process
    for (int i = 0; i < proc_number; i++) {
        if (pid > 0) {
            pid = fork();
            if (!pid) {
                *id = i+1;  //child process handy id
                whoAmI();
                printf("%sChild %d created.\n",RESET, *id);
            }
        }
    }
}

int RandNumbers2file(FILE *fp) {
   srand(time(NULL));
    for (int i = 0; i < N; i++) {
        fprintf(fp, "%d ", rand());
    }
    return 0;
}

float avg(FILE *fp) {
    int number, i = 0;
    float avg = 0;
    while (fscanf(fp, "%d,&number", &number) != EOF) {
        avg += number;
        i++;
    }
    return avg / i;
}
int main() {
    title();
    remove("random.txt");
    int status;
    int id;
    pid_t pid;
    pid = fork();
    FILE *fp;

    if (!pid) {  //child
        whoAmI();
        printf("Creating file..");
        if (fp = fopen("random.txt", "w+")) {
            RandNumbers2file(fp);
            fclose(fp);
             printf("SUCCESS.\n");
            exit(EXIT_SUCCESS);
        } else {
            fclose(fp);
            printf("FAIL.\n");
            exit(EXIT_FAILURE);
        }

    } else if (pid > 0) {  //parent
        whoAmI();
        printf("Waiting for child..");
        wait(&status);
        if (status) {
            printf(RED "Unable to create the file (status %d). Exiting.%s \n\n", status, RESET );
            exit(0);
        } else {
            printf("OK\n");
            createProcesses(3, &id);
            whoAmI();
            printf("My ID is: %d.\n", id);
            if (id == 0) {
                wait(&status);
                wait(&status);
                wait(&status);
                whoAmI();
                printf("All clear, exiting.\n\n");
                exit(0);
            }

            else if (id == 1) {  //child 1
                whoAmI();
                printf("Starting wc -m..\n\n");
                execlp("wc", "wc","-m", "./random.txt", NULL);
            }

            else if (id == 2) {  //child 2
                whoAmI();
                printf("Starting wc -w..\n\n");
                execlp("wc", "wc", "-w", "./random.txt", NULL);

            }

            else if (id == 3) {  //child 3
                if (fp = fopen("random.txt", "r")) {
                    whoAmI();
                    printf("The number avg is %.2f\n\n", avg(fp));
                    fclose(fp);
                } else {
                    whoAmI();
                    printf(RED "Error: unable to open the file random.txt" RESET);
                }
            }
        }
    } else {  //error
        printf(RED "Error during fork() call. Exiting." RESET);
    }
}