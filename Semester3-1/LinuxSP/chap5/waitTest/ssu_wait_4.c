#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin, end;
	pid_t child1, child2;
	int pid, status;

	gettimeofday(&begin, NULL);

	if ((child1 = fork()) == 0) //자식프로세스 생성
		execlp("date", "date", (char *)0);
	
	if ((child2 = fork()) == 0) //자식프로세스 생성
		execlp("who", "who", (char *)0);
	
	printf("parent : waiting for children\n");

	while((pid = wait(&status)) != -1) {
		if(child1 == pid)
			printf("parent : first child: %d\n", (status >> 8)); //첫번째 자식프로세스
		else if (child2 == pid)
			printf("parent : second child: %d\n", (status >> 8)); // 두번째 자식프로세스
	}	
	
	printf("parent : all children terminated\n");

	gettimeofday(&end, NULL);
	ssu_runtime(&begin, &end); //프로그램 수행시간 측정
	exit(0);

}
