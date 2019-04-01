#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin_t, end_t;

	gettimeofday(&begin_t, NULL);

	if(chdir("/etc") < 0) {
		fprintf(stderr, "chdir error\n"); //현재디렉터리를 /etc로 바꾼다. 실패시 에러처리.
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	printf("chdir to /etc succeeded.\n");
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);

}
