#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_time.h"

int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t;
	int fd;

	gettimeofday(&begin_t, NULL);

	if (argc != 3) {
		fprintf(stderr, "usage : %s <oldname> <newname>\n", argv[0]); 
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0) { //oldname 파일 오픈
		fprintf(stderr, "first open error for %s\n", argv[1]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	else
		close(fd); //oldname 파일 닫기

	if(rename(argv[1],argv[2])< 0) { //oldname파일을 newname으로 이름을 바꿈
		fprintf(stderr, "rename error\n");
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0) //newname으로 이름을 바꾸었으므로 해당파일이 열리게 된다
		printf("second open error for %s\n", argv[1]);
	else {
		fprintf(stderr, "it's very strange!\n");
		gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
                exit(1);
	}

	if((fd = open(argv[2], O_RDONLY)) < 0) { 
		fprintf(stderr, "third open error for %s\n", argv[2]);
		gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
                exit(1);
	}
		
	printf("Everything is good!\n");
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);
	
}
