#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

#define BUFFER_SIZE 1024

int main(void) {

	struct timeval begin_t, end_t;
	char buf[BUFFER_SIZE];

	gettimeofday(&begin_t, NULL);
	
	setbuf(stdout, buf); //buf 배열만큼의 배열을 할당
	printf("Hello, ");
	sleep(1);

	printf("OSLAB!!");
	sleep(1);
	printf("\n");
	sleep(1);

	setbuf(stdout, NULL); //아래의 글자들은 1초단위로 끊어서 출력
	printf("How");
	sleep(1);
	printf(" are");
	sleep(1);
	printf(" you?");
	sleep(1);
	printf("\n");
	
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);

}
