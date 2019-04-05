#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

#define BUFFER_SIZE 1024

void ssu_setbuf(FILE *fp, char *buf);

int main(void) {

	struct timeval begin_t, end_t;
	char buf[BUFFER_SIZE];
	char *fname = "/dev/pts/1"; // /dev/pts/1 파일
	FILE *fp;

	gettimeofday(&begin_t, NULL);

	if((fp = fopen(fname, "w")) == NULL) {
		fprintf(stderr, "fopen error for %s", fname);
		gettimeofday(&end_t , NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	ssu_setbuf(fp, buf); //파일 내용을 buf 배열에 할당
	fprintf(fp, "Hello, ");
	sleep(1);
	
	fprintf(fp, "Unix!!");
	sleep(1);

	fprintf(fp, "\n");
	sleep(1);

	ssu_setbuf(fp, NULL); //파일 내용을 출력

	fprintf(fp, "HOW");
	sleep(1);

	fprintf(fp, " ARE");
	sleep(1);

	fprintf(fp, " YOU?");
	sleep(1);

	fprintf(fp, "\n");
	sleep(1); 

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void ssu_setbuf(FILE *fp, char *buf) {

	size_t size;
	int fd;
	int mode;

	fd = fileno(fp);

	if(isatty(fd))
		mode = _IOLBF;

	else
		mode = _IOFBF;

	
	if (buf == NULL) {
		mode = _IONBF;
		size = 0;
	}

	else
		size = BUFFER_SIZE;

	setvbuf(fp, buf, mode, size);

}
