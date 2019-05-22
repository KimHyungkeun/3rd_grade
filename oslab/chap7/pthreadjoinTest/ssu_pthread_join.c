#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *ssu_thread(void *arg);

int main(void) {
    pthread_t tid1, tid2;
    int thread1 = 1;
    int thread2 = 2;
    int status;

    if (pthread_create(&tid1, NULL, ssu_thread, (void *)&thread1) != 0) {
        fprintf(stderr, "pthread_create error\n"); //첫번째 쓰레드 생성
        exit(1);
    }
    
     if (pthread_create(&tid2, NULL, ssu_thread, (void *)&thread2) != 0) {
        fprintf(stderr, "pthread_create error\n"); //두번째 쓰레드 생성
        exit(1);
    }



    pthread_join(tid1, (void *)&status); // 쓰레드1 종료까지 대기
    pthread_join(tid2, (void *)&status); // 쓰레드2 종료까지 대기
    exit(0);
}

void *ssu_thread(void *arg) {
    int thread_index;
    int i;

    thread_index = *((int *)arg);

    for (i = 0 ;i < 5 ; i++) {
        printf("%d : %d\n", thread_index, i); //쓰레드 번호와 현재 값의 상태를 표시
        sleep(1);
    }

    return NULL;
}

