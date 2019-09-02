#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* ssu_thread(void *arg);

int main(void) {

	pthread_t tid;
	pid_t pid;

	if((pthread_create(&tid, NULL, ssu_thread, NULL)) < 0) {
			fprintf(stderr, "pthread error\n");
			exit(1);
	}

	pid = getpid();
	tid = pthread_self();
	printf("Main thread : pid  %d tid  %d\n",(unsigned int)pid,(unsigned int)tid );
	sleep(1);

	exit(0);

}

void *ssu_thread(void* arg) {

	pthread_t tid;
	pid_t pid;

	pid = getpid();
	tid = pthread_self();
	
	printf("New thread : pid  %d tid  %d\n",(unsigned int)pid,(unsigned int)tid );
}
