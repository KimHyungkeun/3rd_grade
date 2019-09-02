#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo) {
	printf("ssu_signal_handler control\n");
}

int main(void) {

	struct sigaction sig_act;
	sigset_t sig_set;

	sigemptyset(&sig_act.sa_mask); //시그널 집합을 전부 비운다.

	sig_act.sa_flags = 0;
	sig_act.sa_handler = ssu_signal_handler;
	sigaction(SIGUSR1, &sig_act, NULL); //SIGUSR1 시그널 발동
	printf("berfore first kill()\n");
	kill(getpid(), SIGUSR1); //SIGUSR1 시그널 전달
	sigemptyset(&sig_set); //시그널 집합 비움
	sigaddset(&sig_set, SIGUSR1); //SIGUSR1 시그널 넣음
	sigprocmask(SIG_SETMASK, &sig_set, NULL);
	printf("berfore second kill()\n");
	kill(getpid(), SIGUSR1); //SIGUSR1 시그널 전달
	printf("after second kill()\n");
	exit(0);
}
