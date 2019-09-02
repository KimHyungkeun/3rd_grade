#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {

	sigset_t old_set;
	sigset_t sig_set;

	sigemptyset(&sig_set); //시그널 집합 비움
	sigaddset(&sig_set, SIGINT); //SIGINT를 넣음
	sigprocmask(SIG_BLOCK, &sig_set, &old_set); //sigset을 블록시킴
	sigsuspend(&old_set); //oldset을 블록시킴과 동시에 대기
	exit(0);

}
