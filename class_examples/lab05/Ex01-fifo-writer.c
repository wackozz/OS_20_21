#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(){

	int fd;
	char arr1[80];


	mkfifo ("aPipe", 0666);

	fd = open ("aPipe", O_WRONLY);
	printf("insert a message to write in the pipe:\n");
	fgets (arr1, 80, stdin);
	//write the input in the arr2 on FIFO and close it.
	write (fd, arr1, strlen (arr1) +1);
	close (fd);

	// to open the fifo as a reader:

	char arr2 [80];
	fd = open ("aPipe", O_RDONLY);
	printf("Waiting to read\n");
	read (fd, arr2, sizeof (arr2));
	printf("Reading from the fifo:%s\n",arr2);
	close (fd);


	return 0;

}
