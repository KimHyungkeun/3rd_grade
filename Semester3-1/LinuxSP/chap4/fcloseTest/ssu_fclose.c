#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin_t, end_t;
	char *fname = "ssu_test.txt";
	FILE *fp;

	gettimeofday(&begin_t, NULL);

	if((fp = fopen(fname, "r")) == NULL) { //ssu_test.txt를 RDONLY 상태로 읽기
		fprintf(stderr, "fopen error for %s\n", fname);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	else {
		printf("Success!\n"); //성공시 메세지 출력
		printf("Opening \"%s\" in \"r\" mode!\n", fname);
	}

	fclose(fp);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
