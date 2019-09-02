#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);


int count = 0;
int input = 0;
int t1 = 0, t2 = 0;

int main(void) {

	pthread_t tid1, tid2;
	int status;

	if (pthread_create(&tid1, NULL, ssu_thread1, NULL) != 0) {
		fprintf(stderr, "pthread_create_error\n");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, ssu_thread2, NULL) != 0) {
		fprintf(stderr, "pthread_create_error\n");
		exit(1);
	}

	printf("Input number(n>2) : ");
	scanf("%d", &input);

	while(1) {

	pthread_join(tid1, (void *)&status);
	pthread_join(tid2, (void *)&status);
	}

	printf("complete\n");
	exit(0);

}

void *ssu_thread1(void *arg) {


	while(1) {
		pthread_mutex_lock(&mutex1);
		if (input < 2) {
		pthread_cond_wait(&cond2, &mutex2);
		}

		if (input == count) {
		pthread_cond_signal(&cond1);
		}

		if (count == 0) {
		t1 += t2;
		count ++;
		printf("Thread 1 : %d\n", t1);
		break;
		}

		else if (count % 2 == 0) {
		t2++;
		count ++;
		printf("Thread 1 : %d", t1);
		}

	}

	pthread_cond_signal(&cond1);
	pthread_cond_wait(&cond2, &mutex2);
	pthread_mutex_unlock(&mutex1);
	
	return NULL;
}

void *ssu_thread2(void *arg) {

	while(1) {
		pthread_mutex_lock(&mutex2);
		if (input < 2) {

		pthread_cond_wait(&cond1, &mutex1);
		}

		if (input == count) {
		pthread_cond_signal(&cond2);
		}

		if (count == 1) {
		t2 += t1;
		count++;
		printf("Thread 2 : %d\n", t2);
		break;
		}

		else if (count % 2 == 1) {
		count++;
		printf("Thread 2 : %d\n", t2);

		}
	}
	pthread_cond_signal(&cond2);
	pthread_cond_wait(&cond1, &mutex1);
	pthread_mutex_unlock(&mutex2);

	return NULL;
}
