#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/wait.h>

double ssu_maketime(struct timeval *time);
void term_stat(int stat);
void ssu_print_child_info(int stat, struct rusage *rusage);

int main(void) {
	struct rusage rusage;
	pid_t pid;
	int status;

	if((pid = fork()) == 0) { //fork execute, child process build
		char *args[] = {"find", "/" ,"-maxdepth", "4","-name", "stdio.h",NULL};

		if(execv("/usr/bin/find", args) < 0){ //에러시 프로그램 종료
			fprintf(stderr, "execv error\n");
			exit(1);
		}
	}

	if (wait3(&status, 0 , &rusage) == pid)
		ssu_print_child_info(status, &rusage);
	else {
		fprintf(stderr, "wait3 error\n");
		exit(1);
	}

	exit(0);
}

double ssu_maketime(struct timeval *time) {
	return ((double)time -> tv_sec + (double)time -> tv_usec/1000000.0); //시간 값을 리턴한다.
}

void term_stat(int stat) {
	if (WIFEXITED(stat))
		printf("normally terminated. exit status = %d\n",WEXITSTATUS(stat)); //exit인자에서 하위 8비트 값을 계싼
	else if (WIFSIGNALED(stat)) //자식프로세스가 시그널을 받았으나 그것을 처리하지 않아 비정상종료 되었으면 참값이다
		printf("abnormal termination by signal %d. %s\n", WTERMSIG(stat), //시그널 번호 리턴
#ifdef WCOREDUMP
		WCOREDUMP(stat)?"core dumped":"no core" //코어 파일이 생성된 경우 참 값을 리턴
#else 
	NULL
#endif
	);

	else if (WIFSTOPPED(stat))
		printf("stopped by signal %d\n", WSTOPSIG(stat));
}

void ssu_print_child_info(int stat, struct rusage *rusage) {
	printf("Termination info follows\n");
	term_stat(stat);
	printf("user CPU time : %.2f(sec)\n", ssu_maketime(&rusage -> ru_utime)); // user CPU시간을 출력
	printf("system CPU time : %.2f(sec)\n", ssu_maketime(&rusage -> ru_stime)); // system CPU시간을 출력

}
