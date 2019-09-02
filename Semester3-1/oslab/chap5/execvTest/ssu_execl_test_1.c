#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	extern char **environ;
	char **str;
	int i;

	for (i=0;i<argc;i++) {
		printf("argv[%d] : %s\n" , i , argv[i]); //argv에 들어있던 리스트들을 출력한다
	}

	for (str = environ ; *str != 0 ; str++) //환경변수들을 출력한다.
		printf("%s\n", *str);

	exit (0);

}
