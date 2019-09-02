#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 5

void *ssu_printhello(void *arg);

int main(void) {

	pthread_t tid[THREAD_NUM];
	int i;

	for (i = 0 ; i < THREAD_NUM ; i++) {

		printf("In main : createing thread %d\n", i);

		if (pthread_create(&tid[i], NULL, ssu_printhello, (void *)&i) != 0) {
		fprintf(stderr, "thread create error\n");
		exit(1);
		}


	}

	pthread_exit(NULL);
	exit(0);
}

void* ssu_printhello(void *arg) {


	int index;
	index = *((int *)arg);

	printf("Hello World! It's me, thread #%d\n", index);
	pthread_exit(NULL);

	return NULL;
}
