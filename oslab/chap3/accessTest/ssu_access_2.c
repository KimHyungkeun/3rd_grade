#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

#define TABLE_SIZE (sizeof(table)/sizeof(*table))

int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t; 
	
	struct {
	char *text;
	int mode;
	} table [] = {
	{"exist",0}, //파일이 존재하면 0
	{"execute",1}, // 실행권한 : 1
	{"write",2}, // 쓰기권한 : 2
	{"read",4}, // 읽기권한 :4
	};
	int i;

	
	gettimeofday(&begin_t, NULL); //시작시간 측정

	if(argc < 2) {
		fprintf(stderr,"usage : %s <file>\n",argv[0]);
		exit(1);
	}

	for(i=0 ; i < TABLE_SIZE ; i++) {
		if(access(argv[1], table[i].mode) != -1) 
			printf("%s -ok\n", table[i].text); //파일이 갖고 있는 권한에 대해서만 ok표기를 한다
		else
			printf("%s\n",table[i].text);
	}


	

	gettimeofday(&end_t, NULL); //종료시간 측정
        ssu_runtime(&begin_t,&end_t); //프로그램 수행시간 측정
	exit(0);



}
