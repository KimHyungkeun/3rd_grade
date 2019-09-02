#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

int main(int argc, char *argv[]) {

	struct timeval begin, end;
	pid_t pid;
	char character, first, last;
	long i;

	gettimeofday(&begin, NULL);
	

	if((pid = fork()) > 0 ) { //부모 프로세스
		first = 'A'; //첫째 문자 
		last = 'Z'; //마지막 문자
	}	

	else if(pid == 0) { //자식 프로세스
		first = 'a'; //첫째 문자
		last = 'z'; //마지막 문자
	}

	else {
		fprintf(stderr, "%s\n", argv[0]); //에러시 출력 후 종료
		gettimeofday(&end, NULL);
		ssu_runtime(&begin, &end);
		exit(1);
	}

	for(character = first ; character <= last ; character++) {
		for(i = 0 ; i <= 100000 ; i++)
			;

		write(1, &character, 1); //부모와 자식 모두 한문자씩 찍게 한다.
	}

	printf("\n");

	gettimeofday(&end, NULL);
	ssu_runtime(&begin, &end);
	exit(0);

}
