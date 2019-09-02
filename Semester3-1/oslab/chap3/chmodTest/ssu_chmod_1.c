#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ssu_time.h"

int main (void) {

	struct stat statbuf;
	struct timeval begin_t, end_t;
	char* fname1 = "ssu_file1";
	char* fname2 = "ssu_file2";

	gettimeofday(&begin_t, NULL);

	if(stat(fname1, &statbuf) < 0)
		fprintf(stderr, "stat error %s\n", fname1);

	if(chmod(fname1, (statbuf.st_mode & ~S_IXGRP) | S_ISUID) < 0) // file1의 S_ISUID를 설정한다
		fprintf(stderr, "chmod error %s\n",fname1);

	if(chmod(fname2, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH) < 0) // file2는 0645모드로 변경한다
		fprintf(stderr, "chmod error %s\n",fname2);

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t,&end_t); //프로그램 수행시간 측정
	exit(0);

}
