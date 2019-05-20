#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *ssu_thread(void *arg);

int main(void) {

	pthread_t tid;
	pid_t pid;

	if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n");//쓰레드 생성
		exit(1);
	}

	pid = getpid(); //현 프로세스 아이디 받아오기
	tid = pthread_self();
	printf("Main Thread : pid %u tid %u \n", (unsigned int)pid, (unsigned int)tid); //메인쓰레드의 프로세스 아이디 출력
	sleep(1);

	exit(0);
}

void *ssu_thread(void *arg) {

	pthread_t tid;
	pid_t pid;

	pid = getpid(); //새로 생성되는 쓰레드의 아이디
	tid = pthread_self();
	printf("New Thread : pid %d tid %u \n", (int)pid, (unsigned int)tid); //새로 생성된 쓰레드의 아이디 출력
	return NULL;
}
