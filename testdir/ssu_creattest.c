#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
	umask(0);
	char *fname = "ssu_creat.txt";
	int fd;

	if((fd = creat(fname,0666)) < 0) {
		fprintf(stderr,"open error for %s\n",fname);
		exit(1);
	}

	else {
		close(fd);
		fd = open(fname,O_RDWR);
		printf("Succeeded!\n<%s> is a new writable and readable\n",fname);
	}
	
	exit(0);

}
