#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_alarm(int signo);

int main(void) {

	printf("Alarm Setting\n");
	signal(SIGALRM, ssu_alarm); //알람 시그널 설정
	alarm(2); //2초뒤 알람 

	while (1) {
		printf("done\n"); 
		pause(); 
		alarm(2); // pause, alarm을 섞어서 sleep과 같은 역할을 한다.
	}

	exit(0);
}

void ssu_alarm(int signo) {
	printf("alarm..!!!\n");
}
