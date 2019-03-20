#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {

        int fd;
        int length;
	off_t offset = atoi(argv[2]);

        if(argc != 4) {
                fprintf(stderr,"Usage : %s file offset data\n",argv[0]);
                exit(1);
        }

        if((fd = open(argv[1],O_RDWR)) < 0) {
                fprintf(stderr,"open error for %s\n",argv[1]);
                exit(1);
        }

	if(lseek(fd, offset, SEEK_SET) < 0) {
		fprintf(stderr,"lseek error\n");
		exit(1);
	}

        write(fd,argv[3],10);

        exit(0);



}

