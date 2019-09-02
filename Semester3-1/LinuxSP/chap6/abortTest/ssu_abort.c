#include <stdio.h>
#include <stdlib.h>

int main(void) {

	printf("abort terminate this program\n");
	abort(); //프로세스 자신에게 SIGABRT 시그널을 발생시키며, 프로그램을 비정상적으로 종료시킨다.
	printf("this line is never reached\n");
	exit(0);
}
