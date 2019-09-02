#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

#define MAX_LINE 4096

int main(void) {

	struct timeval begin_t, end_t;
	char buf[MAX_LINE];
	char name[L_tmpnam];
	FILE *fp;

	gettimeofday(&begin_t, NULL);
	
	printf("temp file 1 : %s\n", tmpnam(NULL));
	tmpnam(name);
	printf("temp file 2 : %s\n", name);

	if((fp = tmpfile()) == NULL) { //임시 파일 생성
		fprintf(stderr, "tmpfile error\n");
		exit(1);
	}

	fputs("tmpfile created temporary file.\n", fp);
	fseek(fp , 0, SEEK_SET); //offset 0으로 위치

	if(fgets(buf, sizeof(buf),fp) == NULL) { 
		fprintf(stderr, "fgets error\n");
		exit(1);
	}

	fputs(buf, stdout); // buf내용 출력
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);

}
