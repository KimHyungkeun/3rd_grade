#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

int main(int argc, char *argv[]) {

	struct timeval begin_t, end_t;

	gettimeofday(&begin_t, NULL);

	if(argc != 3) {
		fprintf(stderr, "usage : %s <oldname> <newname>\n", argv[0]); //명령어와 링크시킬 두 파일을 입력
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	if(link(argv[1], argv[2]) < 0) { //두 파일을 서로 링크시킨다
		fprintf(stderr, "link error\n");
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	else
		printf("step1 passed. \n");

	if(remove(argv[1]) < 0) { //oldname의 파일을 하나 삭제한다.
		fprintf(stderr, "remove error\n"); //삭제 실패시 에러처리
		remove(argv[2]);
		gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	else
		printf("step2 passed.\n");

	printf("Success!\n");
	gettimeofday(&end_t, NULL);
        ssu_runtime(&begin_t, &end_t);
	exit(0);

}
