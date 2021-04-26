#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main (){

	int fd1;
	char str1[80];

	
	// creating the named file (FIFO)
	mkfifo ("aPipe", 0666);

	fd1 = open ("aPipe", O_RDONLY);
	printf("waiting to read:\n");
	read (fd1, str1, 80);

	printf("reading from the pipe: %s\n",str1 );
	close (fd1);

	// now lets write;
	char str2[80];
	fd1 = open ("aPipe", O_WRONLY);
	printf("insert a message to write in the pipe:\n");
	fgets (str2, 80, stdin);
	write (fd1, str2, strlen(str2) + 1);
	close (fd1);

	unlink ("aPipe");
	return 0;

}
