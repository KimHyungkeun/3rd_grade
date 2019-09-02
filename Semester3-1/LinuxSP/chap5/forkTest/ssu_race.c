#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_time.h"

static void ssu_charatatime(char *str);

int main(void) {

	struct timeval begin, end;
	pid_t pid;

	gettimeofday(&begin, NULL);

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n"); //fork error시에러 출력 후 종료
		gettimeofday(&end, NULL);
		ssu_runtime(&begin, &end);
		exit(1);
	}

	else if(pid == 0)
		ssu_charatatime("output from child\n"); //자식 프로세스임을 알림

	else
		ssu_charatatime("output from parent\n"); //부모 프로세스 임을 알림

	gettimeofday(&end, NULL);
        ssu_runtime(&begin, &end);
	exit(0);

}

static void ssu_charatatime(char *str) {
	char *ptr;
	int print_char;

	setbuf(stdout, NULL);

	for(ptr = str; (print_char = *ptr++) != 0; ) { //문자를 하나씩 출력한다.
		putc(print_char, stdout);
		usleep(10);
	}
}
