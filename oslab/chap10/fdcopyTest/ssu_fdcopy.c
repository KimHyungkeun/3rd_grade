#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define MSG "message will be written to Terminal\n"

int main(void) {

	int new_fd;

	if ((new_fd = fcntl(STDOUT_FILENO, F_DUPFD, 3)) == -1) {
		fprintf(stderr, "Error : Copying File Descriptor\n"); //표준출력을 파일디스크립터 3번으로 지정
		exit(1);
	}

	close(STDOUT_FILENO);
	write(3, MSG, strlen(MSG)); //메세지 출력
	exit(0);
}
