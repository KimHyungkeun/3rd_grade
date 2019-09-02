#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void) {

	int flag;

	if((flag = fcntl(STDOUT_FILENO, F_DUPFD)) == -1) {
		fprintf(stderr, "Error : Checking CLOSE_ON_EXEC\n"); //모든 파일디스크립터를 닫는다
		exit(1);
	}

	printf("CLOSE ON EXEC flag is = %d\n", flag);
	exit(0);

}
