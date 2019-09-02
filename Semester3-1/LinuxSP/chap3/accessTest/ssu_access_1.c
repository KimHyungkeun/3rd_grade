#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"


int main(int argc, char* argv[]) {

	struct timeval begin_t, end_t;
	int i;
	
	gettimeofday(&begin_t,NULL); //시작시간 측정


	if(argc < 2) {
		fprintf(stderr,"usage : %s <file1> <file2> .. <fileN>\n",argv[0]);
		gettimeofday(&end_t,NULL);
		ssu_runtime(&begin_t,&end_t); // 프로그램 수행시간 측정
		exit(1);		
	}	

	for ( i=1;i<argc;i++ ) {
		if(access(argv[i],F_OK) < 0) {
			fprintf(stderr, "%s doesn't exist. \n",argv[i]); //파일이 존재하지 않을 경우 출력
			continue;
		}

		if(access(argv[i], R_OK) == 0) {
			printf("User can read %s\n",argv[i]); //파일에 읽기권한이 있을 경우 출력
		}

		if(access(argv[i], W_OK) == 0) {
                        printf("User can write %s\n",argv[i]); //파일에 쓰기권한이 있을 경우 출력
                }

		if(access(argv[i], X_OK) == 0) {
                        printf("User can execute %s\n",argv[i]); //파일에 실행권한이 있을 경우 출력
                }


	}

	 gettimeofday(&end_t,NULL);
         ssu_runtime(&begin_t,&end_t); // 프로그램 수행시간 측정
	 exit(0);
}
