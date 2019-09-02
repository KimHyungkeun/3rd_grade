#include <stdlib.h>
#include <stdio.h>
#include "ssu_time.h"


#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t;
	char buf[BUFFER_SIZE];
	FILE *fp;

	gettimeofday(&begin_t, NULL);
	if(argc != 2) { //인자가 2개가 아닐 경우 에러 출력
		fprintf(stderr, "usage : %s <file>\n", argv[0]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	if((fp = fopen(argv[1], "w+")) == NULL) { //파일을 읽기쓰기모드로 오픈한다
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	fputs("Input String >> ", stdout); //문자열 입력칸
	gets(buf);
	fputs(buf, fp); //문자열 출력
	rewind(fp);
	fgets(buf, sizeof(buf), fp);
	puts(buf);
	fclose(fp);

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
