#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ssu_time.h"

void ssu_addone(void);

extern char **environ;
char glob_var[] = "HOBBY=swimming"; //환경변수 설정

int main(void) {
	
	struct timeval begin_t, end_t;
	int i;
	
	gettimeofday(&begin_t, NULL);
	
	for(i=0 ; environ[i] != NULL ; i++)
		printf("environ[%d] : %s\n", i , environ[i]);

	putenv(glob_var); //"HOBBY 환경변수를 swimming 값을 넣는다"
	ssu_addone();
	printf("My hobby is %s\n", getenv("HOBBY")); //"swimming"이 넣어진다
	printf("My lover is %s\n", getenv("LOVER"));
	strcpy(glob_var + 6, "fishing");

	for ( i = 0 ; environ[i] != NULL ; i++)
		printf("environ[%d] : %s\n", i , environ[i]); //환경변수 출력

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void ssu_addone(void) {

	char auto_var[10];

	strcpy(auto_var, "LOVER=js");
	putenv(auto_var);

}
