#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	
	if(fork() == 0) {
		execl("/bin/echo", "echo", "this is", "message one", (char *)0); //첫번째 메세지 출력
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	if(fork() == 0){
		execl("/bin/echo", "echo", "this is", "message two", (char *)0); //두번째 메세지 출력
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	if(fork() == 0){
		execl("/bin/echo", "echo", "this is", "message three", (char *)0); //세번째 메세지 출력
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	printf("Parent program ending\n"); //부모가 끝나면 이 메세지를 출력한다.
	exit(0);

}
