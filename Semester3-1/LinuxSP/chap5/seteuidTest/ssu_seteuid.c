#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {

	int fd, state;

	state = seteuid(1000); //effective user id를 1000으로 설정

	if (state < 0) {
		fprintf(stderr, "seteuid error\n"); //에러 메세지 출력 후 종료
		exit(1);
	}

	if ((fd = open("ssu_test.txt", O_CREAT | O_RDWR, S_IRWXU)) < 0){
		fprintf(stderr, "open error\n"); //ssu_test.txt를 RWX로 권한으로 만듦
		exit(1); //에러시 종료
	}

	close(fd); //파일 닫기
	exit(0);
}
