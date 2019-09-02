#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(void) {

	char buf[BUFFER_SIZE];
	int pid;
	int pipe_fd[2];

	pipe(pipe_fd);

	if ((pid = fork()) < 0) { //프로세스 생성
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid > 0) {
		printf(" PARENT : writing to the pipe \n"); //부모프로세스
		write(pipe_fd[1], "OSLAB", 6); //표준출력에 OSLAB 설정
		printf(" PARENT : waiting \n");
		wait(NULL);
	}

	else {
		printf(" CHILD : reading from pipe \n"); //자식프로세스
		read(pipe_fd[0], buf, 6); //buf로부터 내용을 읽어온다
		printf(" CHILD : read \"%s\" \n", buf);
		exit(0);
	}

	exit(0);
}
