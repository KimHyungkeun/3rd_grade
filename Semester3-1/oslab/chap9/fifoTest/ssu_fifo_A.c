#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define FILE_MODE S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH
#define FIFO_NAME "ssu_fifofile"

int main(void) {

	char buf[BUFFER_SIZE];
	int fd;
	int length;

	mkfifo(FIFO_NAME, FILE_MODE); //fifo 생성
	printf("waiting for readers... \n");

	if ((fd = open(FIFO_NAME, O_WRONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", FIFO_NAME);
		exit(1); //파일오픈 에러시 종료
	}

	printf("got a reader -- type some stuff \n");

	while (fgets(buf, BUFFER_SIZE, stdin), !feof(stdin)) {
		if ((length = write(fd, buf, strlen(buf) - 1)) == -1) {
			fprintf(stderr, "write error\n"); //입력한 문자열 길이만큼 해당 내용을 출력. 만약 에러가 발생하면 종료
			exit(1);
		}
		else
			printf("speak : wrote %d bytes \n", length); //문자열 길이 출력
	}

	exit(0);
}
