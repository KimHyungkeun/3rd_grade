#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define LINE_MAX 2048

static void ssu_alarm(int signo);


int main(void) {

	char buf[LINE_MAX];
	int n;

	if(signal(SIGALRM, ssu_alarm) == SIG_ERR) { //알람 시그널 에러시
		fprintf(stderr, "SIGALRM error\n"); //에러메세지 출력
		exit(1); //종료
	}

	if((n = read(STDIN_FILENO, buf, LINE_MAX)) < 0 ) { 
		fprintf(stderr, "read() error\n"); //터미널상에서 입력한 문자열을 읽어들임
		exit(1); //실패시 에러 출력 후 종료
	}

	alarm(0);
	write(STDOUT_FILENO, buf, n); // 정상으로 읽었으면 화면에 출력
	exit(0);
}

static void ssu_alarm(int signo) {

	printf("ssu_alarm() called!\n");
}
