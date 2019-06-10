#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(void) {

	int testfd;
	int fd;

	fd = open("ssu_test.txt", O_CREAT);

	testfd = fcntl(fd, F_DUPFD, 5);
	printf("testfd :%d\n", testfd);
	testfd = fcntl(fd, F_DUPFD, 5);
	printf("testfd :%d\n", testfd);

	getchar();
}
