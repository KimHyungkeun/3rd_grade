#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int glo_val1 = 1, glo_val2 = 2;

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

int main(void) {

	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, ssu_thread1, NULL); //쓰레드1 생성
	pthread_create(&tid2, NULL, ssu_thread2, NULL); //쓰레드2 생성
	pthread_join(tid1, NULL); //쓰레드1 조인
	pthread_join(tid2, NULL); //쓰레드2 조인
	pthread_mutex_destroy(&lock); //뮤텍스 해제
	pthread_cond_destroy(&cond); //cond 해제

	exit(0);
}

void *ssu_thread1(void *arg) {

	sleep(1);
	glo_val1 = 2;
	glo_val2 = 1;

	if (glo_val1 > glo_val2)
		pthread_cond_broadcast(&cond); 

	printf("ssu_thread1 end\n");
	return NULL;
}

void *ssu_thread2(void *arg) {

	struct timespec timeout;
	struct timeval now;

	pthread_mutex_lock(&lock); //뮤텍스 잠금
	gettimeofday(&now, NULL); //현재시간 가져오기

	timeout.tv_sec = now.tv_sec + 5;
	timeout.tv_nsec = now.tv_usec + 1000;

	if (glo_val1 <= glo_val2) {

		printf("ssu_thread2 sleep\n");
		if (pthread_cond_timedwait(&cond, &lock, &timeout) == ETIMEDOUT)
			printf("timeout\n"); //5초가 지나도 반응이 없으면 시간초과

		else
			printf("glo_val1 = %d, glo_val2 = %d\n", glo_val1, glo_val2);
	} //전역변수1과 2에 대나 값 출력

	pthread_mutex_unlock(&lock); //뮤텍스 잠금해제
	printf("ssu_thread2 end\n");
	return NULL;
}
