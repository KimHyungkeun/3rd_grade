#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	pid_t pid;

	if ((pid = fork()) < 0) { //자식프로세스 생성 실패시 에러 후 종료
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if(pid != 0) //자식프로세스가 아닌 경우 종료
		exit(1);

	printf("before pid = %d, sid = %d\n", getpid(), getsid(getpid()));
	setsid();
	printf("after pid = %d, sid  %d\n", getpid(), getsid(getpid()));
	exit(0);
}
