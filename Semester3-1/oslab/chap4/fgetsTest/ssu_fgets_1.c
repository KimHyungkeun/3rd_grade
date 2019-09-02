#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

#define BUFFER_SIZE 1024

int main(void) {

	struct timeval begin_t, end_t;
	char buf[BUFFER_SIZE];

	gettimeofday(&begin_t, NULL);
	while(fgets(buf, BUFFER_SIZE, stdin) != NULL) { //문자열을 입력받는다
		if (fputs(buf, stdout) == EOF) { //문자열 출력
			fprintf(stderr, "standard output error\n");
			gettimeofday(&end_t, NULL);
			ssu_runtime(&begin_t, &end_t);
			exit(1);
		}
	}

	if(ferror(stdin)) { //입력 실패시 에러 출력
		fprintf(stderr, "standard input error\n");
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
