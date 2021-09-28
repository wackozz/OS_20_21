#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


#define N 300

int vector[N];

struct indexes {
	int start;
	int end;
};

void merge_branches(struct indexes *arr_indexes) {
	int i;
	int j;
	int tmp_arr[N];
	int a = arr_indexes[0].start;
	int b = arr_indexes[1].start;

	for (i = arr_indexes[0].start; i < arr_indexes[1].end + 1; i++) {
		tmp_arr[i] = vector[i];
	}

	for (i = arr_indexes[0].start; i < arr_indexes[1].end + 1; i++) {
		if (a == arr_indexes[0].end + 1) {
			for (j = b; j < arr_indexes[1].end + 1; j++, i++) {
				vector[i] = tmp_arr[j];
			}
		} else if (b == arr_indexes[1].end + 1) {
			for (j = a; j < arr_indexes[0].end + 1; j++, i++) {
				vector[i] = tmp_arr[j];
			}
		} else { 
			if (tmp_arr[a] < vector[b]) {
				vector[i] = tmp_arr[a];
				a++;
			} else {
				vector[i] = tmp_arr[b];
				b++;
			}
		}
	}
}

void *merge_sort(void *thread_arg) {
	pthread_t thread_id[2];  
	int rc;
	struct indexes arr_indexes[2];
	int tmp;

	struct indexes *thread_indexes;
	thread_indexes = (struct indexes*) thread_arg;
	// printf("%d %d\n", thread_indexes->start, thread_indexes->end);

	int N2 = thread_indexes->end - thread_indexes->start + 1;
	if (N2 > 2) {
		arr_indexes[0].start = thread_indexes->start;
		arr_indexes[0].end = N2 / 2 - 1 + thread_indexes->start;
		arr_indexes[1].start = N2 / 2 + thread_indexes->start;
		arr_indexes[1].end = N2 - 1 + thread_indexes->start;
		rc = pthread_create ( &thread_id[0]  , NULL,  merge_sort, (void *) &arr_indexes[0]);
		if (rc != 0) {
			printf ("ERROR unable to create a new thread\n");
			exit (-1);
		}
		rc = pthread_create ( &thread_id[1]  , NULL,  merge_sort, (void *) &arr_indexes[1]);
		if (rc != 0) {
			printf ("ERROR unable to create a new thread\n");
			exit (-1);
		}
		pthread_join (thread_id[0], NULL);		
		// printf("Thread with ID %lu is terminated\n", thread_id[0]);
		pthread_join (thread_id[1], NULL);		
		// printf("Thread with ID %lu is terminated\n", thread_id[1]);

		merge_branches(arr_indexes);
	} else {
		printf("%d %d: %d %d\n", thread_indexes->start, 
			thread_indexes->end, vector[thread_indexes->start], 
			vector[thread_indexes->end]);
		if (vector[thread_indexes->start] > vector[thread_indexes->end]) {
			tmp = vector[thread_indexes->end];
			vector[thread_indexes->end] = vector[thread_indexes->start];
			vector[thread_indexes->start] = tmp;
		}
		printf("%d %d: %d %d\n", thread_indexes->start, 
			thread_indexes->end, vector[thread_indexes->start], 
			vector[thread_indexes->end]);
	}


}

int main(){
	pthread_t thread_id[2];  
	int rc;
	struct indexes arr_indexes[2];

	// To create the vector:
	srand (time(NULL));

	for (int i = 0; i < N; i ++){
		vector[i] = rand() % 100;
		printf("Vector[%d] = %d\n",i, vector[i]);
	}

	arr_indexes[0].start = 0;
	arr_indexes[0].end = N / 2 - 1;
	arr_indexes[1].start = N / 2;
	arr_indexes[1].end = N - 1;
	rc = pthread_create ( &thread_id[0]  , NULL,  merge_sort, (void *) &arr_indexes[0]);
	if (rc != 0) {
		printf ("ERROR unable to create a new thread\n");
		exit (-1);
	}
	rc = pthread_create ( &thread_id[1]  , NULL,  merge_sort, (void *) &arr_indexes[1]);
	if (rc != 0) {
		printf ("ERROR unable to create a new thread\n");
		exit (-1);
	}
	pthread_join (thread_id[0], NULL);		
	// printf("Thread with ID %lu is terminated\n", thread_id[0]);
	pthread_join (thread_id[1], NULL);		
	// printf("Thread with ID %lu is terminated\n", thread_id[1]);

	merge_branches(arr_indexes);

	for (int i = 0; i < N; i ++){
		printf("Vector[%d] = %d\n", i, vector[i]);
	}
}