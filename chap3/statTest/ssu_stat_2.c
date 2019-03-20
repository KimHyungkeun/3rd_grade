#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ssu_time.h"

struct stat statbuf;

void ssu_checkfile(char *fname, time_t *time);

int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t;
	time_t intertime;
	
	gettimeofday(&begin_t, NULL);

	if(argc != 2) {
		fprintf(stderr, "usage %s <file>\n",argv[0]); //명령어 이외에 인자를 하나 더 넣어야 한다.
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t,&end_t);
		exit(1);
	}

	if(stat(argv[1], &statbuf) < 0) { //만약 해당 파일에 이상이있거나 존재하지 않으면 에러를 출력한다
		fprintf(stderr, "stat error for %s\n",argv[1]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t,&end_t);
		exit(1);
	}
		
	intertime = statbuf.st_mtime; //최종 수정시간을 체크하는 함수
	while(1) {
		ssu_checkfile(argv[1], &intertime); // 파일 체크 함수
		sleep(10);
	}

}

void ssu_checkfile(char *fname, time_t *time) {

	if(stat(fname, &statbuf) < 0) { //파일에 이상이있거나 할 경우 에러를 출력한다.
		fprintf(stderr, "Warning : ssu_checkfile() error\n");
		exit(1);
	}
	

	else
		if(statbuf.st_mtime != *time) { 
			printf("Warning : %s was modified!.\n",fname); //파일이 수정되었을때 이 메세지를 출력한다.
			*time = statbuf.st_mtime; //최종 수정시간을 수정한다.
		}



}
