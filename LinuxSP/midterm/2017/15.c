#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
	struct stat statbuf;

	if (argc != 2) {
		fprintf(stderr, "\n");
		exit(1);
	}

	if (stat(argv[1], &statbuf) < 0) {
		fprintf(stderr, "error\n");
		exit(1);
	}

	printf("%s is %ld bytes\n", argv[1], statbuf.st_size);
	exit(1);

	exit(0);

}


