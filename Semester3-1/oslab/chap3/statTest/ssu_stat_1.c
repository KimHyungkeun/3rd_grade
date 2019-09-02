#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ssu_time.h"

int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t;
	struct stat statbuf;

	gettimeofday(&begin_t,NULL);

	if(argc != 2) {
		fprintf(stderr, "usage : %s <file>\n",argv[0]); //인자를 명령어 말고는 주지 않을 경우 출력
		gettimeofday(&end_t,NULL);
		ssu_runtime(&begin_t,&end_t);
		exit(1);
	}

	if((stat(argv[1], &statbuf)) < 0) { //statbuf 구조체를 참조
		fprintf(stderr, "stat error\n");
		gettimeofday(&end_t,NULL);
                ssu_runtime(&begin_t,&end_t);
		exit(1);
	}

	printf("%s is %ld bytes\n", argv[1], statbuf.st_size); //파일 크기를 출력한다
	gettimeofday(&end_t,NULL);
        ssu_runtime(&begin_t,&end_t); //프로그램 수행시간 측정
	exit(0);

}

