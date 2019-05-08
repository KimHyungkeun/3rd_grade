#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void ssu_signal_handler(int signo);

int main(void) {

	if (signal(SIGINT, ssu_signal_handler) == SIG_ERR) { //Ctrl+C 시그널
		fprintf(stderr, "cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	} 

	if (signal(SIGTERM, ssu_signal_handler) == SIG_ERR) { //kill(1) 명령이 기본적으로 전달하는 종료 시그널
		fprintf(stderr, "cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGPROF, SIG_DFL) == SIG_ERR) {//setitimer로 설정한 프로파일링 구간 타이머가 만료될때 발생, 디폴트액션이므로 프로세스를 종료
		fprintf(stderr, "cannot handle SIGPROF\n");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGPROF, SIG_IGN) == SIG_ERR) {//setitimer로 설정한 프로파일링 구간 타이머가 만료될때 발생, IGN액션이므로 아무런 액션이 없음
		fprintf(stderr, "cannot handle SIGHUP\n");
		exit(EXIT_FAILURE);
	}

	while(1)
		pause();

	exit(0);
}

static void ssu_signal_handler(int signo) {

	if(signo == SIGINT) {
		printf("caught SIGINT\n"); // SIGINT 시그널 잡음
	}

	else if(signo == SIGTERM) {
		printf("caught SIGTERM"); // SIGTERM 시그널 잡음
	}
	
	else {
		fprintf(stderr, "unexpected signal\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
