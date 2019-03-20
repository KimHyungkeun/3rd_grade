#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {

	int fd;
	int length;

	if(argc != 3) {
		fprintf(stderr,"Usage : %s filein\n",argv[0]);
		exit(1);
	}

	if((fd = open(argv[1],O_RDWR | O_APPEND)) < 0) {
		fprintf(stderr,"open error for %s\n",argv[1]);
		exit(1);
	}

	write(fd,argv[2],10);

	exit(0);
	
	

}
