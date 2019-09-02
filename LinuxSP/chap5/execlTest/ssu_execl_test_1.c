#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	extern char **environ;
	char **str;
	int i;

	for (i=0;i<argc;i++) { //arg에 있는 리스트들을 출력한다
		printf("argv[%d] : %s\n" , i , argv[i]);
	}

	for (str = environ ; *str != 0 ; str++) //환경변수들 목록을 출력한다.
		printf("%s\n", *str);

	exit (0);

}
