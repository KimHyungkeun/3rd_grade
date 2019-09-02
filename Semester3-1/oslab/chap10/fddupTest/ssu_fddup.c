#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void) {

	int testfd;
	int fd;

	fd = open("test.txt", O_CREAT); //test.txt 만들기

	testfd = fcntl(fd, F_DUPFD,5); //fd로 파일디스크립터 5번 복사
	printf("testfd : %d\n", testfd);
	testfd = fcntl(fd, F_DUPFD,5); //fd로 파일디스크립터 6번이 복사됨
	printf("testfd : %d\n", testfd);

	getchar();
}
