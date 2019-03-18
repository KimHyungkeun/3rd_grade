#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
	
	int fd1, fd2, length;
	char buf[BUFFER_SIZE];
	
	if(argc != 3) {
		fprintf(stderr,"Usage : %s filein fileout\n",argv[0]);
		exit(1);
	}

	if((fd1 = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr,"open error for %s\n",argv[1]);
		exit(1);
	}

	if((fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC , 0640)) < 0) {
		fprintf(stderr,"creat error for %s\n",argv[2]);
		exit(1);
	}

	if((length = read(fd1, buf, BUFFER_SIZE)) > 0)
		write(fd2, buf, length);

	exit(0);

	
		
}

