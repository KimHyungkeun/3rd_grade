#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define FIFO_NAME "ssu_fifofile"

int main (void) {

	char buf[BUFFER_SIZE];
	int fd;
	int length;

	mknod(FIFO_NAME, S_IFIFO | 0666, 0);
	printf("waiting for writers... \n");

	if ((fd = open(FIFO_NAME, O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", FIFO_NAME);
		exit(1); //파일을 읽기모드로 오픈한다.
	}

	printf("got a writer \n");

	do {
		if ((length = read(fd, buf, BUFFER_SIZE)) == -1) {
			fprintf(stderr, "read error\n"); //파일을 읽어들인다/
			exit(1); //에러시에는 종료
		}
		else {
			buf[length] = '\0'; //문자열 마지막은 널문자로 채움
			printf("tick : read %d bytes : \"%s\" \n", length, buf);
			//읽은 buf의 길이 출력
		}

	}while (length > 0);

	exit(0);
}
