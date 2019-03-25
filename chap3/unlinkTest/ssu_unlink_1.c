#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_time.h"

int main(void) {

        char *fname = "ssu_dump.txt";
        struct timeval begin_t, end_t;

        gettimeofday(&begin_t, NULL);

        if(open(fname, O_RDWR) < 0) { //ssu_dump.txt를 읽기쓰기 가능으로 오픈
                fprintf(stderr, "open error for %s\n", fname);
                gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
                exit(1);
        }

        if(unlink(fname) < 0) {
                fprintf(stderr, "unlink error for %s\n", fname); //unlink error시 에러메세지 출력
                gettimeofday(&end_t, NULL);
                ssu_runtime(&begin_t, &end_t);
                exit(1);
        }

        printf("File unlinked\n");
        sleep(20);
        printf("Done\n");


        gettimeofday(&end_t, NULL);
        ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
        exit(0);

}

