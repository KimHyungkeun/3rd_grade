#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(void) {

	char *fname = "ssu_test.txt";
	int fd;
	char c;

	if((fd = open(fname,O_RDONLY)) < 0) {
		fprintf(stderr,"open error for %s\n",fname);
		exit(1);
	}

	while (read(fd,&c,1) > 0) {
		putchar(c);
	}

	exit(0);
	
}

