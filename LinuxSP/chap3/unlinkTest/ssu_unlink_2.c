#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin_t, end_t;
	char buf[64];
	char *fname = "ssu_tempfile";
	int fd;
	int length;

	gettimeofday(&begin_t, NULL);
	
	if((fd = open(fname, O_RDWR | O_CREAT | O_TRUNC, 0600)) < 0){ //파일 생성 에러처리
		fprintf(stderr, "first open error for %s\n", fname); 
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	if(unlink(fname) < 0) { //unlink할 파일이 없으면 에러처리
		fprintf(stderr, "unlink error for %s\n", fname);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	if(write(fd, "How are you?",12) != 12) { //12byte 읽음
		fprintf(stderr, "write error\n");
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	lseek(fd, 0 ,0); // offset 처음으로 위치


	if((length = read(fd, buf, sizeof(buf))) < 0) { //파일 읽음
		fprintf(stderr, "buf read error\n");
		gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
		exit(1);
	}
	else
		buf[length] = 0;


	printf("%s\n", buf); //buf에 담긴 내용 출력
	close(fd);

	if((fd = open(fname, O_RDWR)) < 0) { //파일 오픈 에러시 에러메세지 출력
		fprintf(stderr, "second open error for %s\n", fname);
		gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	else
		close(fd);

	exit(0);

}
