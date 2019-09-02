#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ssu_time.h"

#define MODE_EXEC (S_IXUSR | S_IXGRP | S_IXOTH) //--x--x--x 0111모드

int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t;
	struct stat statbuf;
	int i;

	gettimeofday(&begin_t, NULL);

	if(argc < 2) {
		fprintf(stderr, "usage : %s <file1> <file2> ... <fileN>\n",argv[0]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);		
	}

	for(i=1;i<argc;i++) {
		if(stat(argv[i], &statbuf) < 0) {
			fprintf(stderr, "%s : stat error\n", argv[i]);
			continue;
		}

		statbuf.st_mode |= MODE_EXEC; //0775 모드 rwxrwxr-x
		statbuf.st_mode ^= (S_IXGRP | S_IXOTH); // 0775 rwxrwxr-x  ->  0764 rwxrw-r-x
		if(chmod(argv[i], statbuf.st_mode) < 0)
			fprintf(stderr, "%s : chmod error\n", argv[i]);
		else
			printf("%s : file permission was changed.\n", argv[i]);
	}
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);
}
