#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin_t, end_t;
	char operator;
	FILE *fp;
	int character;
	int number = 0;

	gettimeofday(&begin_t, NULL);
	
	if((fp = fopen("ssu_expr.txt", "r")) == NULL) { //ssu_expr.txt를 읽기모드로 오픈
		fprintf(stderr, "fopen error for ssu_expr.txt\n");
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	while(!feof(fp)) {
		while((character = fgetc(fp)) != EOF && isdigit(character)) {
			number = 10 * number + character - 48;
		}

		fprintf(stdout, " %d\n", number); //숫자만을 출력한다
		number = 0;

		if(character != EOF) {
			ungetc(character, fp);
			operator = fgetc(fp);
			printf("Operator =? %c\n", operator); //연산자만을 출력한다.
		}
	}

	fclose(fp);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(1);

}
