#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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

	if(lseek(fd, offset, SEEK_SET) < 0) { //지정한 offset으로 이동
		fprintf(stderr,"lseek error\n");
		exit(1);
	}

	if((length = strlen(argv[3])) < 10) //10글자 미만이면 그만큼을 overwrite하기
		write(fd, argv[3], length);

	else
        	write(fd,argv[3],10); // 10글자 이상이면 10글자만큼을 overwrite하기

        exit(0);



}

