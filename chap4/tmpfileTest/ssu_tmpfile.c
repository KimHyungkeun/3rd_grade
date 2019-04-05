#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

#define BUFFER_SIZE 1024

int main(void) {

	struct timeval begin_t, end_t;
	char buf[BUFFER_SIZE];
	char name[L_tmpnam];
	FILE *fp;

	gettimeofday(&begin_t, NULL);

	printf("Temporary filename <<%s>>\n", tmpnam(name));

	if((fp = tmpfile()) == NULL) { //tmp파일 생성
		fprintf(stderr, "tmpfile create error!!\n");
		exit(1);
	}

	fputs("create tmpfile success!!\n", fp); //tmp파일 성공시 메세지 출력
	rewind(fp);
	fgets(buf, sizeof(buf), fp); 
	puts(buf);

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); // 프로그램 수행시간 측정
	exit(0);
	
	

}
