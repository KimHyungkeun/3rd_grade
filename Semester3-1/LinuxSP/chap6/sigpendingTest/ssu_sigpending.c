#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {

	sigset_t pendingset;
	sigset_t sig_set;

	int count = 0;

	sigfillset(&sig_set); //모든 시그널을 sig_set인자가 가리키는 시그널 집합에 포함
	sigprocmask(SIG_SETMASK, &sig_set, NULL); //sig_set을 마스크 시킨다.

	while(1) {

		printf("count :%d\n", count++);
		sleep(1);

		if (sigpending(&pendingset) == 0) { //SIGINT가 들어오면 작동
			if(sigismember(&pendingset, SIGINT)) {
			printf("SIGINT가 블록되어 대기 중. 무한루프 종료.\n");
			break;
			}
		}
	}

	exit(0);
}
