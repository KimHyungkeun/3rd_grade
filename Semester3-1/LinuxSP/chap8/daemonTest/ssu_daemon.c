#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

int ssu_daemon_init(void);

int main(void) {

	pid_t pid;

	pid = getpid();
	printf("parent process : %d\n", pid);
	printf("daemon process initialization\n");

	if (ssu_daemon_init() < 0) { //디몬 초기화
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(1);
	}

	exit(0);
}

int ssu_daemon_init(void) {

	pid_t pid;
	int fd, maxfd;

	if ((pid = fork()) < 0) { //규칙1 자식프로세스 생성
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if(pid != 0)
		exit(0);

	pid = getpid();
	printf("process %d running as daemon\n", pid);
	setsid(); //규칙2 기존 그룹을 탈퇴하고 새 프로세스 그룹 지정
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN); //규칙3 터미널 입출력 시그널 무시
	signal(SIGTSTP, SIG_IGN);
	maxfd = getdtablesize();

	for (fd = 0 ; fd < maxfd ; fd++)
		close(fd); //규칙 6 오픈되는 모든 파일디스크립터 닫기

	umask(0); //규칙 4 파일 모드 생성 마스크 해제
	chdir("/"); //규칙 5 현재 디렉터리를 루트로 설정
	fd = open("/dev/null", O_RDWR); //규칙 7 표준입출력과 표준에러를 dev/null로 재지정 
	dup(0);
	dup(0);
	return 0;
}
