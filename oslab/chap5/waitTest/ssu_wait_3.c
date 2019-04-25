#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ssu_time.h"

int main(void) {
	
	struct timeval begin, end;
	gettimeofday(&begin,NULL);

	if (fork() == 0) //자식프로세스 일때 execl 실행
		execl("/bin/echo", "echo", "this is", "message one", (char *)0);

	if (fork() == 0) //자식프로세스 일때 execl 실행
		execl("/bin/echo", "echo", "this is", "message Two", (char *)0);

	printf("parent : waiting for children\n"); //자식프로세스 wait

	while (wait((int *)0) != -1);

	printf("parent : all childeren terminated\n"); //자식프로세스 kill

	gettimeofday(&end, NULL);
	ssu_runtime(&begin, &end); //프로그램 실행시간 측정
	exit(0);

}

