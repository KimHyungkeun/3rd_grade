#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

#define BUFFER_SIZE 1024

int main(void) {

	struct timeval begin_t, end_t;
	char *fname = "ssu_test.dat";
	char name[BUFFER_SIZE];
	FILE *fp;
	int age;

	gettimeofday(&begin_t, NULL);	

	fp = fopen(fname, "r"); //파일을 읽기모드로 오픈
	fscanf(fp, "%s%d", name, &age); //이름과 나이 입력
	fclose(fp); //파일 닫음
	fp = fopen(fname, "w"); //파일을 쓰기모드로 오픈
	fprintf(fp, "%s is %d years old\n", name, age); //입력한 값들을 출력
	fclose(fp); //파일 닫음
	
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
