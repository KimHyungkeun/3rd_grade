#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 8

struct thread_data {
	int thread_index;
	int sum;
	char* message;
};

void* ssu_printhello(void *arg);

struct thread_data thread_data_array[THREAD_NUM];
char *messages[THREAD_NUM];

int main(void) {

	pthread_t tid[THREAD_NUM];
	int sum;
	int i;

	sum = 0;
	messages[0] = "Hello World!";
	messages[1] = "Bonjour";
	messages[2] = "Hola al mundo";
	messages[3] = "Nuq neH!";
	messages[4] = "Guten Tag!";
	messages[5] = "Zdravstvytye";
	messages[6] = "konnichiwa";
	messages[7] = "Orbis";

	for (i=0;i<THREAD_NUM;i++) {
		sum = sum + i;
		thread_data_array[i].sum = sum;
		thread_data_array[i].thread_index = i;
		thread_data_array[i].message = messages[i];
		printf("Createing thread : %d\n", i);

		if (pthread_create(&tid[i], NULL, ssu_printhello,(void *) &thread_data_array[i]) != 0) {
			fprintf(stderr, "pthread create error\n");
			exit(1);
		}
	}

	sleep(5);
	exit(0);
}

void *ssu_printhello(void* arg) {

	struct thread_data *data;
	data = (struct thread_data *)arg;

	sleep(1);
	printf("Thread : %d : %s sum = %d\n", data -> thread_index, data ->message, data -> sum);

}
