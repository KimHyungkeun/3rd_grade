#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t;
	gettimeofday(&begin_t, NULL);

	if(argc != 3) {
		fprintf(stderr, "usage : %s <actualname> <sysname>\n", argv[0]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	if(symlink(argv[1],argv[2]) < 0) {
		fprintf(stderr, "symlink error\n"); //actualname을 sysname 이름으로  symbolic link 방식으로 연결한다. 실패시 에러출력
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	else
		printf("symlink: %s -> %s\n", argv[2], argv[1]); //링크된 모습을 출력

	exit(0);

}
