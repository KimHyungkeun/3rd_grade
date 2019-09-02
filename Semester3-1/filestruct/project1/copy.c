#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 2048

int main(int argc, char* argv[]) {

	char buf[BUFFER_SIZE];
	int origin_fd, copy_fd, length;

	if (argc != 3) {
		fprintf(stderr,"Usage : %s originfile copyfile",argv[0]);
		exit(1);
	}

	if((origin_fd = open(argv[1],O_RDONLY)) < 0) {
		fprintf(stderr,"open error for %s\n",argv[1]);
		exit(1);
	}

	if((copy_fd = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) { //copy파일을 생성한다
		fprintf(stderr, "open error for %s\n",argv[2]);
	}	

	while((length = read(origin_fd, buf, 100)) > 0) { //100byte씩 읽어서 복사한다.
		write(copy_fd, buf, length);
	}

	exit(1);


}
