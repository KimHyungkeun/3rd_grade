#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

int main(int argc, char *argv[]) {

	struct timeval begin_t, end_t;
	char *arg_directory = NULL;
	char *arg_prefix = NULL;
	
	gettimeofday(&begin_t, NULL);

	if(argc != 3) { //인자가 3개가 아니면 에러 출력
		fprintf(stderr, "usage : %s <directory> <prefix>\n", argv[0]);
		exit(1);
	}

	arg_directory = argv[1][0] != ' ' ? argv[1] : NULL; //공백이면 NULL 반환
	arg_prefix = argv[2][0] != ' ' ? argv[2] : NULL; //공백이면 NULL 반환
	printf("created : %s\n", tempnam(arg_directory, arg_prefix));// tmp파일 생성 성공시 출력

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

	

}
