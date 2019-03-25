#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ssu_time.h"

#define RW_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH | S_IWOTH)
//0666 모드

int main(void) {

	struct timeval begin_t, end_t;
	char *fname1 = "ssu_file1";
	char *fname2 = "ssu_file2";

	gettimeofday(&begin_t, NULL);
	umask(0); //umask 없이 진행하겠단 뜻이다

	if(creat(fname1, RW_MODE) < 0) {
		fprintf(stderr, "creat error for %s\n",fname1);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); //umask를 0066으로 설정

	if(creat(fname2, RW_MODE) < 0) {
		fprintf(stderr, "creat error for %s\n",fname2);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);

}

