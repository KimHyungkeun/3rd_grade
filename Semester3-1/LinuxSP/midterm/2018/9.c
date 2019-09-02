#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 128
#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char* argv[]) {

	char buf[BUFFER_SIZE];
	int fd1, fd2;
	ssize_t size;

	if(argc < 3) {
		fprintf(stderr, "Usage : %s <oldfile> <newfile>", argv[0]);
		exit(1);
	}	

	if((fd1 = open(argv[1], O_RDONLY , MODE)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	size = read(fd1, buf, BUFFER_SIZE);
	close(fd1);

	if((fd2 = open(argv[2], O_WRONLY, MODE )) < 0) {
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}

	write(fd2, buf, size);
	close(fd2);
	exit(0);
}
