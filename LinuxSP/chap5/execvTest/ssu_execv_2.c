#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

	char *argv[] = { "ssu_execl_test_1", "param1", "param2", (char*) 0}; //argv에 다음 목록들을 넣는다
	
	printf("this is the original program\n");
	execv("./ssu_execl_test_1", argv); //목록들을 보여준다
	printf("%s\n", "This line should never get printed\n");
	exit(0);
}
