#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 2048

int main(int argc, char* argv[]) {
	
 	char buf[BUFFER_SIZE];
	int fd;
	int length;
	off_t offset = atoi(argv[2]); //문자열을 숫자로 바꿔주는 함수
	
	
	if(argc != 3) {
		fprintf(stderr,"Usage : %s filename offset\n",argv[0]);
		exit(1);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr,"open error for %s\n",argv[1]);
		exit(1);
	}

 	if(lseek(fd, offset, SEEK_SET) < 0 ) {
		fprintf(stderr, "lseek error\n");
		exit(1);
	}	
	
	length = read(fd,buf,10); //문자열을 지정한 offset부터 10글자 읽는다.
	write(1, buf, length);
	printf("\n");
	
	exit(0);
}
