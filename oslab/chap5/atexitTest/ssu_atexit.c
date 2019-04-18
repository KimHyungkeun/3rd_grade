#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

void ssu_out(void);

int main(void) {

	struct timeval begin_t, end_t;
	gettimeofday(&begin_t, NULL);

	if (atexit(ssu_out)) { // ssu_out 함수를 호출한다
		fprintf(stderr, "atexit error\n"); // 에러 발생시 에러 출력
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1); //종료
	}

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void ssu_out(void) {
	printf("atexit succeeded!\n");

}
