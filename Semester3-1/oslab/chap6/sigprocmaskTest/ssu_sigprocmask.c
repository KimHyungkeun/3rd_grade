#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {

	sigset_t sig_set;
	int count;

	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGINT); //sigset에 SIGINT시그널을 add
	sigprocmask(SIG_BLOCK, &sig_set, NULL); 

	for (count = 3 ; 0 < count ; count--) {
		printf("count %d\n", count); //count를 실시한다
		sleep(1);
	}

	printf("Ctrl-C에 대한 블록을 해제\n");
	sigprocmask(SIG_UNBLOCK, &sig_set, NULL); //ctrl-c 를 입력시키면
	printf("count중 Ctrl-C입력하면 이 문장은 출력 되지 않음.\n"); //이 문장이 출력

	while(1);

	exit(0);

}
