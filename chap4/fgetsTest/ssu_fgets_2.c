#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

#define BUFFER_MAX 256

int main(void) {

	struct timeval begin_t, end_t;
	char command[BUFFER_MAX];
	char *prompt = "Prompt>>";

	gettimeofday(&begin_t, NULL);

	while(1) {

	fputs(prompt, stdout); //프롬프트 출력

	if(fgets(command, sizeof(command), stdin) == NULL) {
		break; //입력받은 문자열이 NULL이면  반복문 탈출
	}

	system(command);	

	}

	fprintf(stdout, "Good bye...\n"); //종료 메세지 출력
	fflush(stdout);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);



}
