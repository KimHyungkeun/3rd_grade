#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread(void *arg);

int main(void) {

	pthread_t tid;

	if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n"); //쓰레드 생성
		exit(1);
	}

	printf("%u\n", (unsigned int)tid); //생성된 쓰레드의 아이디

	if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n"); //쓰레드 생성
		exit(1);
	}

	printf("%u\n", (unsigned int)tid); //생성된 쓰레드의 아이디 
	sleep(1);
	exit(0);
}

void *ssu_thread(void *arg) {

	pthread_t tid;

	tid = pthread_self(); //현재의 쓰레드 아이디를 리턴한다
	printf("->%u\n", (unsigned int)tid);
	return NULL;
}
