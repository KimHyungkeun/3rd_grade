#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_loop1(void *arg);
void *ssu_loop2(void *arg);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_value;

int main(void) {

	pthread_t tid1, tid2;
	int status;

	shared_value = 0;

	if (pthread_create(&tid1, NULL, ssu_loop1, NULL) != 0) {
		fprintf(stderr, "pthread_create_error\n"); //쓰레드1 생성
		exit(1);
	}

	sleep(1);

	if (pthread_create(&tid2, NULL, ssu_loop2, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n"); ///쓰레드2 생성
		exit(1);
	}

	if (pthread_join(tid1, (void *)&status) != 0) {
		fprintf(stderr, "pthread_join error\n"); //쓰레드1 조인
	}

	if (pthread_join(tid2, (void *)&status) != 0) {
		fprintf(stderr, "pthread_join error\n"); //쓰레드2 조인
	}

	status = pthread_mutex_destroy(&mutex); //뮤텍스 사용후 해제
	printf("code = %d\n", status);
	printf("programming is end\n");
	exit(0);
}

void *ssu_loop1(void *arg) {

	int i;

	for (i=0;i<10;i++) {
		pthread_mutex_lock(&mutex); //뮤텍스 잠금
		printf("loop1 : %d\n", shared_value);
		shared_value++; //공유되는 변수

		if (i == 10)
			return NULL;

		pthread_mutex_unlock(&mutex); //뮤텍스 잠금해제
		sleep(1);
	}

	return NULL;
}

void *ssu_loop2(void *arg) {

	int i;
	
	for (i=0;i<10;i++) {
		pthread_mutex_lock(&mutex); //뮤텍스 잠금
		printf("loop2 : %d\n", shared_value);
		shared_value++; //공유되는 변수
		pthread_mutex_unlock(&mutex); //뮤텍스 잠금 해제
		sleep(2);
	}

	return NULL;
}
