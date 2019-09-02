#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_MAX 4

int main(int argc, char *argv[]) {

	int fd;
	size_t n;
	int count = 0;
	char buf[2], cbuf[5];

	if (argc != 2) {
		fprintf(stderr,"usage : %s <filename>\n", argv[0]);
		exit(1);
	}

	

}
