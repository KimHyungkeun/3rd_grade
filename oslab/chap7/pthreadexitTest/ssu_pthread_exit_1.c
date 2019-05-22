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
        printf("In main : creating thread %d\n", i);

        if (pthread_create(&tid[i], NULL, ssu_printhello, (void*)&i) != 0) {
            fprintf(stderr, "pthread_create error\n");//쓰레드 생성. 실패시 에러 출력 후 종료
            exit(1);
        }
    }

    pthread_exit(NULL); //쓰레드 종료
    exit(0);
}

void *ssu_printhello(void *arg) {
    int thread_index;

    thread_index = *((int *)arg); //몇번째 쓰레드인지 index 변수에 넣음
    printf("Hello World! It's me, thread #%d!\n", thread_index);
    pthread_exit(NULL); //쓰레드 종료

    return NULL;
}

