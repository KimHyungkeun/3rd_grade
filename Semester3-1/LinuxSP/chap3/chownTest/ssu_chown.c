#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ssu_time.h"

int main(void) {
		
	struct stat statbuf;
	struct timeval begin_t, end_t;
	char *fname = "ssu_myfile";
	int fd;

	gettimeofday(&begin_t, NULL);

	if((fd = open(fname, O_RDWR | O_CREAT, 0600)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	close(fd);
	stat(fname, &statbuf);
	printf("# 1st stat call # UID:%d  GID:%d\n", statbuf.st_uid, statbuf.st_gid); //변경전의 UID와 GID를 표시

	if(chown(fname, 501, 300) < 0) { //UID 를 501로, GID를 300으로 변경
		fprintf(stderr, "chown error for %s\n", fname);
		gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	stat(fname, &statbuf);
	printf("# 2nd stat call # UID:%d  GID:%d\n", statbuf.st_uid, statbuf.st_gid);

	if(unlink(fname) < 0) { //open한 파일에 대한 링크를 끊는다.
		fprintf(stderr, "unlink error for %s\n", fname);
		gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	gettimeofday(&end_t, NULL);
        ssu_runtime(&begin_t, &end_t);
	exit(0);
}	
