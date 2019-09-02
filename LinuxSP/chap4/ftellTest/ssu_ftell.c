#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin_t, end_t;
	char *fname = "ssu_test.txt";
	FILE *fp;
	long fsize;

	gettimeofday(&begin_t, NULL);	

	if((fp = fopen(fname, "r")) == NULL) { //파일을 읽기모드로 오픈
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	fseek(fp , 0, SEEK_END); //파일크기 측정
	fsize = ftell(fp);
	printf("The size of <%s> is %ld bytes\n", fname, fsize); //출력메세지
	
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);
	
}
