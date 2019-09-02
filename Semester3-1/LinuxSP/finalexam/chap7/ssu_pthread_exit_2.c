#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_pthread(void *arg);

int main(void) {

	pthread_t tid;

	if (pthread_create(&tid, NULL, ssu_pthread, NULL) != 0) {
		fprintf(stderr, "thread create error\n");
		exit(1);
	}

	sleep(1);

	printf("쓰레드가 완료되기전 main함수가 먼저 종료되면 실행 중 쓰레드 소멸\n");
	printf("메인 종료\n");
	exit(0);
}

void* ssu_pthread(void *arg) {

	printf("쓰레드 시작\n");
	sleep(5);
	printf("쓰레드 수행 완료\n");
	pthread_exit(NULL);
	return NULL;
}
