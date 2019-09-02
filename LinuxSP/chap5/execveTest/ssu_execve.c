#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

	char *argv[] = {"ssu_execl_test_1", "param1", "param2", (char *) 0}; //argv에 각각의 문자열들을 넣는다
	char *env[] = { "NAME = value", "nextname=nextvalue","HOME=/home/oslab",(char *)0}; //환경변수의 목록들이다

	printf("this is the original program\n");
	execve("./ssu_execl_test_1", argv, env); //실행
	printf("%s\n", "This line should never get printed\n");
	exit(0);

}
