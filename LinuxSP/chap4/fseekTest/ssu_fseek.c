#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin_t, end_t;
	char *fname = "ssu_test.txt";
	FILE *fp;
	long position;
	int character;
	
	gettimeofday(&begin_t, NULL);

	if((fp = fopen(fname, "r")) == NULL) { //파일을 읽기모드로 오픈
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	printf("Input number >>"); 
	scanf("%ld", &position); //숫자 입력
	fseek(fp, position - 1, SEEK_SET); //입력으로 들어온 값만큼 떨어진 위치의 문자 한개를 호출
	character = getc(fp);
	printf("%ldth character => %c\n", position, character); //출력 메세지

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
