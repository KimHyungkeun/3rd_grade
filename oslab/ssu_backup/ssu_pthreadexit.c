#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


/*void *ssu_thread(void *arg);

int main(void) {
    pthread_t tid;
    
    if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
   
    sleep(1);
    printf("쓰레드가 완료되기 전 main 함수가 먼저 종료되면 실행중 쓰레드 소멸\n");
    printf("메인 종료\n");
    exit(0);
    
}

void *ssu_thread(void *arg) {
   
   printf("Thread Start\n");
   sleep(5);
   printf("Thread Complete\n");
   pthread_exit(NULL);
   
    return NULL;
}*/

#define THREAD_NUM 5 

void *ssu_printhello(void *arg);

int main(void) {
    pthread_t tid[THREAD_NUM];
    int i;

    for (i = 0 ; i < THREAD_NUM ; i++) {
        printf("In main : creating thread %d\n", i);

        if (pthread_create(&tid[i], NULL, ssu_printhello, (void*)&i) != 0) {
            fprintf(stderr, "pthread_create error\n");
            exit(1);
        }
    }

    pthread_exit(NULL);
    exit(0);
}

void *ssu_printhello(void *arg) {
    int thread_index;

    thread_index = *((int *)arg);
    printf("Hello World! It's me, thread #%d!\n", thread_index);
    pthread_exit(NULL);

    return NULL;
}

