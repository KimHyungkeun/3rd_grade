#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(void) {

	sigset_t set;

	sigemptyset(&set); //시그널 집합을 빈 집합으로 만듦
	sigaddset(&set, SIGINT); //SIGINT를 시그널 집합에 추가

	switch(sigismember(&set, SIGINT)) { //SIGINT가 시그널 집합에 속하는지 확인

		case 1 :
			printf("SIGINT is included. \n"); //속하므로 이 문장이 출력된다.
			break;
		case 0 :
			printf("SIGINT is not included. \n");
			break;
		default :
			printf("failed to call sigismember() \n");

	}

	switch(sigismember(&set, SIGSYS)) { //SIGSYS가 시그널 집합에 속하는지 확인

		case 1 :
			printf("SIGSYS is included. \n");
			break;
		case 0 :
			printf("SIGSYS is not included. \n"); //속하지 않으므로 이 문장이 출력
			break;
		default :
			printf("failed to call sigismember() \n");

	}
}
