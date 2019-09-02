#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

char glob_str[] = "write to standard output\n";
int glob_val = 10; //전역변수를 10으로 설정

int main(void) {

	struct timeval begin, end;
	pid_t pid;
	int loc_val;

	gettimeofday(&begin, NULL);

	loc_val = 100; //지역변수를 100으로 설정
	if (write(STDOUT_FILENO, glob_str, sizeof(glob_str)-1) != sizeof(glob_str) - 1) {
		fprintf(stderr, "write error\n"); //write 에러시 출력
		gettimeofday(&end, NULL);
		ssu_runtime(&begin, &end);
		exit(1);
	}

	printf ("before fork\n"); // before fork

	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n"); //pid가 0보다 작으면 에러 출력
		exit(1);
	}

	else if(pid == 0) { //자식 프로세스의 경우
		glob_val++; //전역변수 1 증가
		loc_val++; //지역변수 1 증가
	}
	else
		sleep(3); //부모프로세스의 경우 3초 sleep

	printf("pid = %d, glob_val = %d, loc_val = %d\n",getpid(), glob_val, loc_val);
	gettimeofday(&end,NULL);
	ssu_runtime(&begin, &end);
	exit(0);
}
