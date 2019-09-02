#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

#define BUFFER_SIZE 1024

int main(void) {

	struct timeval begin_t, end_t;
	char buf[BUFFER_SIZE];
	int a,b;
	int i;

	gettimeofday(&begin_t, NULL);

	setbuf(stdin, buf); //buf 배열에 정수 2개를 각각 입력한다
	scanf("%d %d", &a, &b);

	for(i=0;buf[i] != '\n'; i++)
		putchar(buf[i]); //입력받은 값들을 출력한다.

	putchar('\n');

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
