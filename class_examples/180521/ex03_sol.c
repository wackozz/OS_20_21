
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>


void childs (sem_t S1, int *shm){//child of child:
	int r;
	srand(getpid());
	while(1){
		r = rand ()%5 +1;
		printf ("Sleeping the child of the child for R= %d\n", r);
		sleep (r);
		sem_wait (&S1);
		printf ("child of the child using sv\n");

		if (*shm == -1){
			sem_post (&S1);
		}
		else {
			printf("value of SV reached to child of child: sv=%d\n", *shm );
			*shm = -1;
			printf("value of SV replaced by child of child: sv=%d\n", *shm );
			sem_post (&S1);
			shmdt (shm);
			exit (0);
		}
	}
}



int main (){

	pid_t pid;
	int sv =-1;
	int r;

	//Preparing Semaphore:
	sem_t S1;
	sem_init (&S1, 0 , 1);
	printf ("Semaphore generated\n");

//preparing shared memory:
	int shmid;
	int *shm;
	shmid = shmget (IPC_PRIVATE, sizeof (int), IPC_CREAT | IPC_EXCL | 0666);
	if (shmid == -1){
		perror ("Error in shmat:");
		exit (errno);
	}
	shm = (int*) shmat (shmid, NULL, 0);
	if (shm == (int*)-1){
		sem_destroy (&S1);
		shmctl (shmid, IPC_RMID, 0);
		perror ("Error in shmat");
		exit (errno);
	}
	printf ("Shared memory generated\n");


	*shm = sv;

	//Parent generating the first child:
	pid = fork ();
	printf ("First child generated\n");
	if (pid <0 ){
		perror ("Error in fork:");
		exit(errno);
	}
	else if (pid ==0){ //first child:
		srand (getpid());
		while(1){
			r = rand ()%3+1;
			printf ("Sleeping the the child of parent for R= %d\n", r);
			sleep (r);
			pid = fork();
			if (pid <0){
				perror("Error in the second fork():");
				exit (errno);
			}
			else if (pid == 0){
				childs (S1,shm);
			}
		}
	}
	else { //Parent
		srand (getpid());
		while (1){
			r = rand ()%10 +1;
			printf ("Sleeping the the parent for R= %d\n", r);
			sleep (r);
			sem_wait (&S1);
			printf ("parent using sv\n");
			printf ("value of SV reaching to parent:%d\n", *shm );
			*shm=r;
			printf ("value of SV replaced by parent:%d\n", *shm );
			sem_post (&S1);
		}
	}
	return 0;

}