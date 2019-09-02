#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

	printf("this is the origial program\n");
	execl("./ssu_execl_test_1", "ssu_execl_test_1", "param1", "param2", "param3", (char *)0); //해당 리스트 목록들을 execl 한다. execl이므로 리스트들을 넘겨주는 것이다 
	printf("%s\n", "this line should never get printed\n");
	exit(0);

}

