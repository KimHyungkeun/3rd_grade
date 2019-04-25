#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "ssu_time.h"

void ssu_local_func(void);
struct timeval begin, end;

int main(void) {
	
	//gettimeofday(&begin, NULL);
	
	printf("Before vfork\n");
	ssu_local_func(); //vfork 관련 함수
	printf("After ssu_local_func, my PID is %d\n", getpid());

	//gettimeofday(&end, NULL);
	//ssu_runtime(&begin, &end);
	exit(0); //함수 종료
}

void ssu_local_func(void) {
	pid_t pid;

	if((pid = vfork()) == 0) { //자식 프로세스
		printf("I'm child. My PID is %d\n", getpid());
	}

	else if(pid > 0) { //부모 프로세스
		sleep(3);
	}

	else {
		fprintf(stderr, "vfork error\n"); //포크 에러
	}
	

}
