#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin_t, end_t;
	int character;

	gettimeofday(&begin_t, NULL);
	while((character = getc(stdin)) != EOF) { //문자 입력하기
		if(putc(character, stdout) == EOF) { //문자 출력 
			fprintf(stderr, "standard output error\n");
			gettimeofday(&end_t, NULL); //문자입력 실패시 프로그램 종료
			ssu_runtime(&begin_t, &end_t);
			exit(1);
		}
	}
	

	if(ferror(stdin)) {
		fprintf(stderr, "standard input error\n"); //문자 입력 실패시 에러출력
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
