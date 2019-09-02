#include <stdio.h>
#include <unistd.h>

int main(void) {

	printf("Good afternoon?");
	_exit(0); //printf가 나오기도 전에 바로 종료된다

}
