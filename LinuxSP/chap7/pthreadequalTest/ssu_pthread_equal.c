#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread(void *arg);

pthread_t glo_tid;

int main(void) {

	pthread_t loc_tid;

	if (pthread_create (&loc_tid, NULL, ssu_thread, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n");//쓰레드변수를 지역변수에 할당, 에러시 에러메세지 출력 후 종료
		exit(1);
	}

	sleep(5);

	if (pthread_equal(loc_tid, glo_tid) == 0) { //지역쓰레드와 전역 쓰레드를 서로 비교
		printf("다른 쓰레드\n");
		exit(0);
	}

	printf("동일한 쓰레드\n");
	exit(0);
}

void *ssu_thread(void *arg) {

	printf("쓰레드에서 자신의 쓰레드 ID를 전역변수에 할당 \n");
	glo_tid = pthread_self(); //쓰레드변수를 전역변수에 할당
	return NULL;
}
