#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo); //핸들러 함수

int count = 0;

int main(void) {

	signal(SIGALRM, ssu_signal_handler); //시그널을 보내고
	alarm(1); //1초뒤 알람 셋팅

	while(1);

	exit(0);
}

void ssu_signal_handler(int signo) { //핸들러 함수

	printf("alarm %d\n", count++);
	alarm(1);
}
