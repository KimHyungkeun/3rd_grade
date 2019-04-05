#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t;
	FILE *fp;
	int character;

	gettimeofday(&begin_t, NULL);
	if(argc != 2) { //인자가 2개가 아닐경우 에러처리
		fprintf(stderr, "usage : %s <filename>\n", argv[0]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	if((fp = fopen(argv[1], "r")) == NULL) { // 파일을 읽기모드로 오픈한다
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	character = fgetc(fp);

	while(!feof(fp)) {

		fputc(character, stdout); //문자를 출력한다

		if(ferror(fp)) { //에러 발생시 하단의 문자열 출력
			fprintf(stderr, "Error detected!\n");
			clearerr(fp);
		}

		character = fgetc(fp);
	}

	fclose(fp);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
