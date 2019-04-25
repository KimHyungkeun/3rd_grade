#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ssu_time.h"

#define EXIT_CODE 1

int main(void) {

	struct timeval begin, end;
	pid_t pid;
	int ret_val, status;

	gettimeofday(&begin, NULL);
	if((pid = fork()) == 0) { //자식프로세스
		printf("child : pid = %d ppid = %d exit_code = %d\n", getpid(), getppid(), EXIT_CODE); //자식프로세스와 부모프로세스 번호를 출력
		exit(EXIT_CODE);
	}

	printf("parent : waiting for child = %d\n", pid); //프로세스 wait
	ret_val = wait(&status);
	printf("parent : return value = %d, ", ret_val); //부모프로세스 값
	printf(" child's status = %x", status); //자식프로세스 값
	printf(" and shifted = %x\n", (status >> 8));
	gettimeofday(&end, NULL);
	ssu_runtime(&begin, &end);
	exit(0);

}
