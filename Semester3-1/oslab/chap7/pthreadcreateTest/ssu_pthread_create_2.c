#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 8

struct thread_data {
    int thread_index;
    int sum;
    char *message;
};

void *ssu_printhello(void *arg); //지정한 메세지를 출력하기 위한 함수

struct thread_data thread_data_array[THREAD_NUM]; //메세지들을 담기위한 배열
char *messages[THREAD_NUM];

int main(void) {
    pthread_t tid[THREAD_NUM];
    int sum;
    int i;
    
    sum = 0;
    messages[0] = "English: Hello World!";
    messages[1] = "French : Bonjour, le monde!";
    messages[2] = "Spanish: Hola al mundo";
    messages[3] = "Klingon: Nuq neH!";
    messages[4] = "German : Guten Tag, Welt!";
    messages[5] = "Russian : Zdravstvytye, mir!";
    messages[6] = "Japan : Sekai e knnichiwa!";
    messages[7] = "Latin : Orbis, te saluto!";

    for (i = 0 ; i < THREAD_NUM ; i++) {
        sum = sum + i;
        thread_data_array[i].thread_index = i;
        thread_data_array[i].sum = sum;
        thread_data_array[i].message = messages[i];
        printf("Creating thread %d\n", i); //배열 하나하나에 메세지들과 sum값들얼 넣는다.

        if (pthread_create(&tid[i], NULL, ssu_printhello, (void*)&thread_data_array[i]) != 0) {
            fprintf(stderr, "pthread_create error\n"); //쓰레드를 생성한다.
            exit(1);
        }
    }
   
    sleep(5); //5초간 일시정지
    exit(0);
}

void *ssu_printhello(void *arg) {
    struct thread_data *data;
    char *message;
    int thread_index;
    int sum;

    sleep(1);
    data = (struct thread_data *)arg;
    thread_index = data -> thread_index; //쓰레드 인덱스
    sum = data -> sum; //n번째 배열의 sum 값
    message = data -> message; //n번째 배열의 message
    printf("Thread %d: %s Sum=%d\n", thread_index, message, sum);

    return NULL;
}

