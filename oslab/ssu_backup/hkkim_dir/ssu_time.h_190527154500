#include <stdio.h>
#include <sys/time.h>

#define SECOND_TO_MICRO 1000000

void ssu_runtime (struct timeval* begin_t, struct timeval* end_t) {  //프로그램 수행 시간을 측정하기 위한 함수

        end_t -> tv_sec -= begin_t -> tv_sec;

        if(end_t -> tv_usec < begin_t -> tv_usec) {
                end_t -> tv_sec--;
                end_t -> tv_usec += SECOND_TO_MICRO;
        }

        end_t -> tv_usec -= begin_t -> tv_usec;
        printf("Runtime : %ld:%06ld(sec:usec)\n",end_t -> tv_sec, end_t -> tv_usec); //sec와 msec출력
}

