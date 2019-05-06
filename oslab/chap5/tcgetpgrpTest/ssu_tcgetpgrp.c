#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>

#define STDIN_NUMBER 0

static void ssu_sig_hup(int signum);
static void ssu_print_ids(char *name);

int main(void) {

	pid_t pid;
	char character;

	ssu_print_ids("parent"); //parent를 이름으로 둔다
	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid > 0) { //부모프로세스이면
		sleep(3); //3초간 대기후
		exit(0); //종료
	}

	else {
		ssu_print_ids("child"); //child를 이름으로 둔다
		signal(SIGHUP, ssu_sig_hup);//연결이 끊어졌음을 제어 터미널 인터페이스가 검출했을 때 제어 터미널에 연고나도니 제어프로세스로 SIGHUP 시그널이 전달
		kill(getpid(), SIGTSTP);  //프로세스 죽임
		ssu_print_ids("child"); 

		if(read(STDIN_FILENO, &character, 1) != 1)
			fprintf(stderr, "read error\n"); //읽기 실패시 에러출력

		exit(0);
	}
}

static void ssu_sig_hup(int signum) {
	printf("ssu_SIG_HUP received, pid = %d\n", getpid()); //SIGHUP 받음
}

static void ssu_print_ids(char* name) {
	printf("[%s] : pid = %d, ppid = %d, pgrp = %d, tpgrp = %d\n", name, getpid(), getppid(), getpgrp(), tcgetpgrp(STDIN_NUMBER));
	fflush(stdout);
}
