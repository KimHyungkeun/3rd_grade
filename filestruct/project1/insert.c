#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {

	char buf[BUFFER_SIZE];
        int fd;
        int length;
	int i;
	off_t offset = atoi(argv[2]);

        if(argc != 4) {
                fprintf(stderr,"Usage : %s file offset data\n",argv[0]);
                exit(1);
        }

        if((fd = open(argv[1],O_RDWR)) < 0) {
                fprintf(stderr,"open error for %s\n",argv[1]);
                exit(1);
        }


	length = read(fd, buf, BUFFER_SIZE);
	for(i = length-1 ; i >= offset ; i--) {
		buf[i+10] = buf[i];
		buf[i] = ' ';
	}
	lseek(fd, 0, SEEK_SET);
	write(fd,buf,length+10);

	
	if(lseek(fd, offset, SEEK_SET) < 0) {
		fprintf(stderr,"lseek error\n");
		exit(1);
	}
        write(fd,argv[3],10);

        exit(0);



}

