#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

	struct flock lock;
	int fd;
	char command[100];

	if ((fd = open(argv[1], O_RDWR)) == -1) {
		perror(argv[1]);
		exit(1);
	}

	lock.l_type = F_WRLCK; //타 프로세스는 읽기쓰기 락 모두 불가능
	lock.l_whence = 0; //SEEK
	lock.l_start = 0l; //바이트 단위 오프셋
	lock.l_len = 0l; //EOF 락해제

	if (fcntl(fd, F_SETLK, &lock) == -1) {

		if (errno == EACCES) {
			printf("%s busy -- try later\n", argv[1]);
			exit(2);
		}
		perror(argv[1]); //파일에러시 종료
		exit(3);
	}

	sprintf(command, "vim %s\n", argv[1]); //vi 실행
	system(command);
	lock.l_type = F_UNLCK; //락해제
	fcntl(fd, F_SETLK, &lock); //락 설정
	close(fd);

	return 0;
}
