#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_FILENAME 100
//global variables
int N;
int producer_counter = 0;
int verifier_counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;
char * filename[MAX_FILENAME];
//struct
typedef struct{
char product[11];
int error;
int prod;
} data;
//functions
void randomStr(char* str){
//strlen check to avoid segfault
if(strlen(str)!=10){
strcpy(str,"\0")
return str;
}
int prob=0;
//probability to generate a number (error) equal to 1/8
for(int i = 0; i<10; i++){
prob = rand()%8
if (prob == 0){
str[i] = (char) (rand()%10+'0'); //from 0 to 9
}
else{
str[i] = (char) (rand()%26+'a') //from a to z,
}
}
str[11]='\0';
}
int checkString(char * str){
//here goes the same initial check of randomStr()
int error=0;
for(int i=0; i<10; i++){
if(isDigit(str[i])){
error = 1;
break;
}
return error;
}
int howManyOkChar(char *str){
//same check as the above f.
int ok=0;
for(int i=0;i<10;i++){
if(isDigit(str[i])){
error++;
}
return 10-error;
}
//thread functions
void * producer(void * arg){
bin = (data*) arg;
char * str[11] = "0000000000"; //creating a 10 char str
for(int i=0; i<10; i++){
pthread_mutex_lock(&mutex);
randomStr(str); //random str
strcpy(&bin[i]->product,str);
bin[i]->prod= 1;
producer_counter++;
pthread_mutex_cond_signal(&cond);
pthread_mutex_unlock(&mutex);
sleep(rand()%(N/2);
}
pthread_exit(NULL)
}
void * verifier (void * arg){
data *bin = (data*) arg;
for(verifier_counter = 0; verifier_count < 10*((N-2)/2);verifier_coun++){
pthread_mutex_lock(&mutex);
while(bin[verifier_counter].prod->0){
pthread_cond_wait(&cond,&mutex);
}
bin[verifier_counter]->error = checkStr(&bin[verifier_counter].product);
pthread_mutex_unlock(&mutex);
sleep(rand()%(N/2)+1)
}
pthread_exit(NULL)
}
void * storageFacility(void * arg){
data* bin = (data*) arg;
FILE *fp;
fp = fopen(filename,"w+"); //create files, overwrite it if it exists
fclose(fp);
for(int i=0;i< 10*((N-2)/2); i++){
pthread_mutex_lock(&mutex);
while(bin[i].prod->0){
pthread_cond_wait(&cond,&mutex);
}
fopen(filename,"a");
fprintf(fp,"%s\n",&bin[i]->product);
fclose(fp);
pthread_mutex_unlock(&mutex);
}
pthread_exit(NULL)
}
void * wasteManagement(void *arg){
data * bin = (data*) arg;
for(int i=0, i<10*((N-2)/2);i++){
pthread_mutex_lock(&mutex);
while(bin[i].prod->0){
pthread_cond_wait(&cond,&mutex);
}
printf("String: %s, tot. %d char ok",&bin[i]->product,howManyOkChar(&bin[i]->product)));
pthread_mutex_unlock(&mutex);
}
pthread_exit(NULL)
}

int main(int argc, char *argv[]){
/*
I'm omitting to control the return value of the various function (*_create, init)
due to time reason.
*/
//srand init
srand(time(NULL));
//check argc
if(argc<3){
printf("ERROR: wrong argument number. Usage: %s N filename",argv[0]);
exit(EXIT_FAILURE);
}
//static variables
N = atoi(argv[1]);
//verify if N is even or odd, if odd N rounded to the next even
if(N%2){
N++;
}
strcpy(filename,argv[2]); //copy filename into global variable
//dynamic memory alloc
pthread_t *tid = (pthread_t*) malloc(((N-2)/2)*10*sizeof(pthread_t)); //10*numofproducer
data *arg = (data *) malloc(N*sizeof(data));
//initializations
pthread_mutex_init(&mutex, NULL);
pthread_cond_init(&cond,NULL);
for(int i=0; i<((N-2)/2)*10; I++){
bin[i]->error = 0;
bin[i]->ver = 0;
}
//producer creation
for(int i=0; i < (N-2)/2;i++){
pthread_create(tid+i,NULL,producer,(void*)arg);
}
//verifier creation
for(int i=(N-2)/2; i < N-1; i++){
pthread_create(tid+i, NULL, producer, (void*) arg);
}
//storageFacility creation
pthread_create(tid+(N-1),NULL,storageFacility,(void*)arg);
//wasteManagement creation
pthread_create(tid+N,NULL,wasteManagement,(void*)arg);

//thread join
for(int i=0; i<N;i++){
pthread_join(*(tid+i),NULL)
}
//free dynamic mem, destroy
free(data);
free(tid);
pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&cond);
}