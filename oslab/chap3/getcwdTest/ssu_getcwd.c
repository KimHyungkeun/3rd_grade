#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

#define PATH_MAX 1024

int main(void) {

	struct timeval begin_t, end_t;
	char *pathname;

	gettimeofday(&begin_t, NULL);

	if(chdir("/home/hkkim/oslab") < 0) { // home/hkkim/oslab 를 현재 디렉터리로 함
		fprintf(stderr, "chdir error\n");
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
		exit(1);
	}

	pathname = malloc(PATH_MAX);

	if(getcwd(pathname, PATH_MAX) == NULL) { //현재 작업 디렉토리에 대한 대한 전체 경로 이름을 얻을 수 있는 시스템 호출함수
		fprintf(stderr, "getcwd error\n");
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	printf("current directory = %s\n", pathname);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
