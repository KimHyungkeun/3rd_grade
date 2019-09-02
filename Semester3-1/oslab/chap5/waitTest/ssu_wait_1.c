#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ssu_time.h"

void ssu_echo_exit(int status);

int main(void) {

	pid_t pid;
	struct timeval begin, end;
	int status;

	gettimeofday(&begin, NULL);

	if((pid = fork()) < 0) { //fork 실패시 에러
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if (pid == 0) // pid 0이면 종료
		exit(7);

	if(wait(&status) != pid) { //wait error 발생시 종료
		fprintf(stderr, "wait error\n");
		exit(1);
	}

	ssu_echo_exit(status);
	
	if((pid = fork()) < 0) { //fork 실패시 에러
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid == 0) 
		abort();

	if(wait(&status) != pid) { //wait error 발생시 종료
		fprintf(stderr, "wait error\n");
		exit(1);
	}

	ssu_echo_exit(status);	
	
	if((pid = fork()) < 0) { //fork 실패시 에러 출력
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid == 0) 
		status /= 0; //status를 0으로 나눔. 비정상종료를 유도

	if(wait(&status) != pid) {
		fprintf(stderr, "wait error\n"); //wait error 출력
		exit(1);
	}

	ssu_echo_exit(status);
	gettimeofday(&end, NULL);
	ssu_runtime(&begin, &end);
	exit(1);	
}

void ssu_echo_exit(int status) {
	if (WIFEXITED(status)) //자식 프로세스가 정상 종료시 참
		printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //exit()인자에서 하위 8비트값 리턴
	else if (WIFSIGNALED(status)) //자식 프로세스가 시그널을 받았으나 그걸 처리하지 않아 비정상적으로 종료되었으면 참
		printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), //시그널 번호 리턴
#ifdef WCOREDUMP
		WCOREDUMP(status) ? " (core file getnerated)" : ""); //코어파일이 생성된 경우에 참값 리
#else
		"");
#endif
	else if (WIFSTOPPED(status)) //자식프로세스가 현재 중지상태 이면 참
		printf("child stopped, signal number = %d\n", WSTOPSIG(status)); //실행을 일시 중단시킨 시그널 번호 리턴

}
