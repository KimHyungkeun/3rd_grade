#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void ssu_signal_handler(int signo);
void ssu_timestamp(char *str);

int main(void) {

	struct sigaction sig_act;
	sigset_t blk_set;

	sigfillset(&blk_set); //시그널 집합을 모두 채운다
	sigdelset(&blk_set, SIGALRM); //SIGALRM 시그널을 지운다
	sigemptyset(&sig_act.sa_mask); //시그널 집합 비우기
	sig_act.sa_flags = 0;
	sig_act.sa_handler = ssu_signal_handler; //핸들러 함수
	sigaction(SIGALRM, &sig_act, NULL); //SIGALRM 시그널 설정
	ssu_timestamp("before sigsuspend()");
	alarm(5);

	sigsuspend(&blk_set); //blk_set에 있는 시그널 대기
	ssu_timestamp("after sigsuspend()"); //경과시간 출력
	exit(0);
}

void ssu_signal_handler(int signo) {

	printf("in ssu_signal_handler() fucntion\n");
}

void ssu_timestamp(char *str) {

	time_t time_val;

	time(&time_val);
	printf("%s the time is %s\n", str, ctime(&time_val));
}
